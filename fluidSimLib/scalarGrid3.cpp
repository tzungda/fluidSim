//---scalarGrid3.cpp

#include "scalarGrid3.h"

scalarGrid3::scalarGrid3()
{
}

scalarGrid3::~scalarGrid3()
{
}


void scalarGrid3::resize(const size3& resolution, const vector3 &gridSpacing,
    const vector3& origin, FloatType initValue)
{
    setSize(resolution, gridSpacing, origin);

    mData.resize(dataSize(), initValue);
    resetSampler();
}


scalarGrid3::DataPositionFunc scalarGrid3::dataPosition() const
{
    vector3 o = dataOrigin();
    return [this, o](size_t i, size_t j, size_t k) -> vector3 {
        return o + gridSpacing() * vector3({ (FloatType)i, (FloatType)j, (FloatType)k });
    };
}

vector3 scalarGrid3::gradientByIndex(size_t i, size_t j, size_t k) const
{
    const size3 s = mData.size();

    FloatType left = mData((i > 0) ? i - 1 : i, j, k);
    FloatType right = mData((i + 1 < s.x) ? i + 1 : i, j, k);
    FloatType down = mData(i, (j > 0) ? j - 1 : j, k);
    FloatType up = mData(i, (j + 1 < s.y) ? j + 1 : j, k);
    FloatType back = mData(i, j, (k > 0) ? k - 1 : k);
    FloatType front = mData(i, j, (k + 1 < s.z) ? k + 1 : k);

    return 0.5 * vector3(right - left, up - down, front - back) / gridSpacing();
}

vector3 scalarGrid3::gradientAtPoint(const vector3& x) const
{
    std::array<size3, 8> indices;
    std::array<FloatType, 8> weights;
    this->mLinearSampler.getCoordinatesAndWeights(x, &indices, &weights);

    vector3 result;
    for (int i = 0; i < 8; ++i)
    {
        result += weights[i] * gradientByIndex(indices[i].x, indices[i].y, indices[i].z);
    }
    return result;
}

FloatType scalarGrid3::laplacianByIndex(size_t i, size_t j, size_t k) const
{
    const FloatType center = mData(i, j, k);
    const size3 s = mData.size();
    const vector3& gs = gridSpacing();

    FloatType dleft = 0.0;
    FloatType dright = 0.0;
    FloatType ddown = 0.0;
    FloatType dup = 0.0;
    FloatType dback = 0.0;
    FloatType dfront = 0.0;

    if (i > 0)
        dleft = center - mData(i - 1, j, k);
    if (i + 1 < s.x)
        dright = mData(i + 1, j, k) - center;

    if (j > 0)
        ddown = center - mData(i, j - 1, k);
    if (j + 1 < s.y)
        dup = mData(i, j + 1, k) - center;

    if (k > 0)
        dback = center - mData(i, j, k - 1);
    if (k + 1 < s.z)
        dfront = mData(i, j, k + 1) - center;

    return (dright - dleft) / (gs.x*gs.x) + (dup - ddown) / (gs.y*gs.y) + (dfront - dback) / (gs.z*gs.z);
}


void scalarGrid3::forEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    mData.forEachIndex(func);
}

dataBuffer3& scalarGrid3::data()
{
    return mData;
}

const dataBuffer3& scalarGrid3::data() const
{
    return mData;
}


FloatType& scalarGrid3::operator()(size_t i, size_t j, size_t k)
{
    return mData(i, j, k);
}

const FloatType& scalarGrid3::operator()(size_t i, size_t j, size_t k) const
{
    return mData(i, j, k);
}


void scalarGrid3::resetSampler()
{
    mLinearSampler = LinearBufferSampler(
        &mData, gridSpacing(), dataOrigin());

    mSampler = mLinearSampler.functor();
}

FloatType scalarGrid3::sample(const vector3 &x) const
{
    return mSampler(x);
}

std::function<FloatType(const vector3&)> scalarGrid3::sampler() const
{
    return mSampler;
}

void scalarGrid3::fill(FloatType value)
{
    for (size_t i = 0; i < mData.width(); ++i)
    {
        for (size_t j = 0; j < mData.height(); ++j)
        {
            for (size_t k = 0; k < mData.depth(); ++k)
            {
                mData(i, j, k) = value;
            }
        }
    }
}

void scalarGrid3::fill(const std::function<FloatType(const vector3&)>& func)
{
    DataPositionFunc pos = dataPosition();
    for (size_t i = 0; i < mData.width(); ++i)
    {
        for (size_t j = 0; j < mData.height(); ++j)
        {
            for (size_t k = 0; k < mData.depth(); ++k)
            {
                mData(i, j, k) = func(pos(i, j, k));
            }
        }
    }
}


void scalarGrid3::swapScalarGrid(scalarGrid3* other)
{
    swapGrid(other);

    mData.swap(other->mData);
    std::swap(mLinearSampler, other->mLinearSampler);
    std::swap(mSampler, other->mSampler);
}

void scalarGrid3::setScalarGrid(const scalarGrid3& other)
{
    setGrid(other);

    mData.set(other.mData);
    resetSampler();
}

void scalarGrid3::getData(std::vector<FloatType>* data) const 
{
    size_t size = dataSize().x * dataSize().y * dataSize().z;
    data->resize(size);
    std::copy(mData.begin(), mData.end(), data->begin());
}

void scalarGrid3::setData(const std::vector<FloatType>& data) 
{
    std::copy(data.begin(), data.end(), mData.begin());
}


