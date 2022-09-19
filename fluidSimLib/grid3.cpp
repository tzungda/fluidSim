//---grid3.cpp

#include "grid3.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>

grid3::grid3()
{
    mGridSpacing = vector3(1.0, 1.0, 1.0);
}

grid3::~grid3()
{
}

const size3& grid3::resolution() const
{
    return mResolution;
}

const vector3& grid3::origin() const
{
    return mOrigin;
}

const vector3& grid3::gridSpacing() const
{
    return mGridSpacing;
}

const boundingBox3& grid3::boundingBox() const
{
    return mBoundingBox;
}

void grid3::setSize(const size3& resolution, const vector3& gridSpacing, const vector3& origin)
{
    mResolution = resolution;
    mOrigin = origin;
    mGridSpacing = gridSpacing;

    vector3 upper(gridSpacing.x * (FloatType)resolution.x + origin.x, gridSpacing.y * (FloatType)resolution.y + origin.y, gridSpacing.z * (FloatType)resolution.z + origin.z);
    mBoundingBox = boundingBox3(origin, upper);
}


vector3 grid3::cellCenterPosition(SizeType i, SizeType j, SizeType k) const
{
    const vector3& h = mGridSpacing;
    const vector3& o = mOrigin;
    return vector3(o.x + (i + (FloatType)0.5) * h.x, o.y + (j + (FloatType)0.5) * h.y, o.z + (k + (FloatType)0.5) * h.z);
}

grid3::DataPositionFunc grid3::cellCenterPosition() const
{
    vector3 h = mGridSpacing;
    vector3 o = mOrigin;
    return [h, o](SizeType i, SizeType j, SizeType k) {
        return o + h * vector3(i + (FloatType)0.5, j + (FloatType)0.5, k + (FloatType)0.5);
    };
}

void grid3::forEachCellIndex(
    const std::function<void(SizeType, SizeType, SizeType)>& func) const
{
    for (SizeType k = 0; k < mResolution.z; ++k)
    {
        for (SizeType j = 0; j < mResolution.y; ++j) 
        {
            for (SizeType i = 0; i < mResolution.x; ++i) 
            {
                func(i, j, k);
            }
        }
    }
}

void grid3::swapGrid(grid3* other) 
{
    std::swap(mResolution, other->mResolution);
    std::swap(mGridSpacing, other->mGridSpacing);
    std::swap(mOrigin, other->mOrigin);
    std::swap(mBoundingBox, other->mBoundingBox);
}

void grid3::setGrid(const grid3& other)
{
    mResolution = other.mResolution;
    mGridSpacing = other.mGridSpacing;
    mOrigin = other.mOrigin;
    mBoundingBox = other.mBoundingBox;
}


