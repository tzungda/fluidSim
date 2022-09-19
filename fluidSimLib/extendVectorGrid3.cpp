//---extendVectorGrid3.cpp

#include "extendVectorGrid3.h"

extendVectorGrid3::extendVectorGrid3()
{
}

extendVectorGrid3::~extendVectorGrid3()
{
}


FloatType extendVectorGrid3::divergenceByIndex(SizeType i, SizeType j, SizeType k) const
{
    const size3 s = mData.size();
    const vector3& gs = gridSpacing();


    FloatType left = mData((i > 0) ? i - 1 : i, j, k).x;
    FloatType right = mData((i + 1 < s.x) ? i + 1 : i, j, k).x;
    FloatType down = mData(i, (j > 0) ? j - 1 : j, k).y;
    FloatType up = mData(i, (j + 1 < s.y) ? j + 1 : j, k).y;
    FloatType back = mData(i, j, (k > 0) ? k - 1 : k).z;
    FloatType front = mData(i, j, (k + 1 < s.z) ? k + 1 : k).z;

    return (FloatType)0.5 * (right - left) / gs.x + (FloatType)0.5 * (up - down) / gs.y + (FloatType)0.5 * (front - back) / gs.z;
}


vector3 extendVectorGrid3::curlByIndex(SizeType i, SizeType j, SizeType k) const
{
    const size3 s = mData.size();
    const vector3 &gs = gridSpacing();

    vector3 left = mData((i > 0) ? i - 1 : i, j, k);
    vector3 right = mData((i + 1 < s.x) ? i + 1 : i, j, k);
    vector3 down = mData(i, (j > 0) ? j - 1 : j, k);
    vector3 up = mData(i, (j + 1 < s.y) ? j + 1 : j, k);
    vector3 back = mData(i, j, (k > 0) ? k - 1 : k);
    vector3 front = mData(i, j, (k + 1 < s.z) ? k + 1 : k);

    FloatType fx_y_lower = down.x;
    FloatType fx_y_upper = up.x;
    FloatType fx_z_lower = back.x;
    FloatType fx_z_upper = front.x;

    FloatType fy_x_lower = left.y;
    FloatType fy_x_upper = right.y;
    FloatType fy_z_lower = back.y;
    FloatType fy_z_upper = front.y;

    FloatType fz_x_lower = left.z;
    FloatType fz_x_upper = right.z;
    FloatType fz_y_lower = down.z;
    FloatType fz_y_upper = up.z;

    return vector3( (FloatType)0.5 * (fz_y_upper - fx_y_lower) / gs.y - (FloatType)0.5 * (fy_z_upper - fy_z_lower) / gs.z,
        (FloatType)0.5 * (fx_z_upper - fx_z_lower) / gs.z - (FloatType)0.5 * (fz_x_upper - fz_x_lower) / gs.x,
        (FloatType)0.5 * (fy_x_upper - fy_x_lower) / gs.x - (FloatType)0.5 * (fx_y_upper - fx_y_lower) / gs.y);
}

void extendVectorGrid3::forEachDataPointIndex(
    const std::function<void(SizeType, SizeType, SizeType)>& func) const
{
    mData.forEachIndex(func);
}

vectorGrid3::DataPositionFunc extendVectorGrid3::dataPosition() const
{
    vector3 dataOrigin_ = dataOrigin();
    return [this, dataOrigin_](SizeType i, SizeType j, SizeType k) -> vector3 
    {
        return dataOrigin_ + gridSpacing() * vector3((FloatType)i, (FloatType)j, (FloatType)k);
    };
}

vecDataBuffer3& extendVectorGrid3::data()
{
    return mData;
}

const vecDataBuffer3& extendVectorGrid3::data() const
{
    return mData;
}

vector3& extendVectorGrid3::operator() (SizeType i, SizeType j, SizeType k)
{
    return mData(i, j, k);
}

const vector3& extendVectorGrid3::operator() (SizeType i, SizeType j, SizeType k) const
{
    return mData(i, j, k);
}

vector3 extendVectorGrid3::sample(const vector3& x) const
{
    return mSampler(x);
}

void extendVectorGrid3::swapExtendVectorGrid(extendVectorGrid3* other)
{
    swapGrid(other);

    mData.swap(other->mData);
    std::swap(mLinearSampler, other->mLinearSampler);
    std::swap(mSampler, other->mSampler);
}

void extendVectorGrid3::setExtendVectorGrid(const extendVectorGrid3& other)
{
    setGrid(other);

    mData.set(other.mData);
    resetSampler();
}

void extendVectorGrid3::getData(std::vector<FloatType>* data) const
{
    SizeType size = 3 * dataSize().x * dataSize().y * dataSize().z;
    data->resize(size);
    SizeType cnt = 0;
    mData.forEach([&](const vector3& value) {
        (*data)[cnt++] = value.x;
        (*data)[cnt++] = value.y;
        (*data)[cnt++] = value.z;
    });
}

void extendVectorGrid3::setData(const std::vector<FloatType>& data)
{
}

void extendVectorGrid3::onResize(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& origin,
    const vector3& initialValue) 
{

    mData.resize(dataSize(), initialValue);
    resetSampler();
}

void extendVectorGrid3::resetSampler() 
{
    mLinearSampler = LinearVecBufferSampler(
        &mData, gridSpacing(), dataOrigin());
    mSampler = mLinearSampler.functor();
}


