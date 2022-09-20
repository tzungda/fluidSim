//---extendVectorGrid3.cpp

#include "extendVectorGrid3.h"

extendVectorGrid3::extendVectorGrid3()
{
}

extendVectorGrid3::~extendVectorGrid3()
{
}


double extendVectorGrid3::divergenceByIndex(size_t i, size_t j, size_t k) const
{
    const size3 s = mData.size();
    const vector3& gs = gridSpacing();


    double left = mData((i > 0) ? i - 1 : i, j, k).x;
    double right = mData((i + 1 < s.x) ? i + 1 : i, j, k).x;
    double down = mData(i, (j > 0) ? j - 1 : j, k).y;
    double up = mData(i, (j + 1 < s.y) ? j + 1 : j, k).y;
    double back = mData(i, j, (k > 0) ? k - 1 : k).z;
    double front = mData(i, j, (k + 1 < s.z) ? k + 1 : k).z;

    return 0.5 * (right - left) / gs.x + 0.5 * (up - down) / gs.y + 0.5 * (front - back) / gs.z;
}


vector3 extendVectorGrid3::curlByIndex(size_t i, size_t j, size_t k) const
{
    const size3 s = mData.size();
    const vector3 &gs = gridSpacing();

    vector3 left = mData((i > 0) ? i - 1 : i, j, k);
    vector3 right = mData((i + 1 < s.x) ? i + 1 : i, j, k);
    vector3 down = mData(i, (j > 0) ? j - 1 : j, k);
    vector3 up = mData(i, (j + 1 < s.y) ? j + 1 : j, k);
    vector3 back = mData(i, j, (k > 0) ? k - 1 : k);
    vector3 front = mData(i, j, (k + 1 < s.z) ? k + 1 : k);

    double fx_y_lower = down.x;
    double fx_y_upper = up.x;
    double fx_z_lower = back.x;
    double fx_z_upper = front.x;

    double fy_x_lower = left.y;
    double fy_x_upper = right.y;
    double fy_z_lower = back.y;
    double fy_z_upper = front.y;

    double fz_x_lower = left.z;
    double fz_x_upper = right.z;
    double fz_y_lower = down.z;
    double fz_y_upper = up.z;

    return vector3(0.5 * (fz_y_upper - fx_y_lower) / gs.y - 0.5 * (fy_z_upper - fy_z_lower) / gs.z,
        0.5 * (fx_z_upper - fx_z_lower) / gs.z - 0.5 * (fz_x_upper - fz_x_lower) / gs.x,
        0.5 * (fy_x_upper - fy_x_lower) / gs.x - 0.5 * (fx_y_upper - fx_y_lower) / gs.y);
}

void extendVectorGrid3::forEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mData.forEachIndex(func);
}

vectorGrid3::DataPositionFunc extendVectorGrid3::dataPosition() const
{
    vector3 dataOrigin_ = dataOrigin();
    return [this, dataOrigin_](size_t i, size_t j, size_t k) -> vector3 
    {
        return dataOrigin_ + gridSpacing() * vector3((double)i, (double)j, (double)k);
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

vector3& extendVectorGrid3::operator() (size_t i, size_t j, size_t k)
{
    return mData(i, j, k);
}

const vector3& extendVectorGrid3::operator() (size_t i, size_t j, size_t k) const
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

void extendVectorGrid3::getData(std::vector<double>* data) const
{
    size_t size = 3 * dataSize().x * dataSize().y * dataSize().z;
    data->resize(size);
    size_t cnt = 0;
    mData.forEach([&](const vector3& value) {
        (*data)[cnt++] = value.x;
        (*data)[cnt++] = value.y;
        (*data)[cnt++] = value.z;
    });
}

void extendVectorGrid3::setData(const std::vector<double>& data)
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


