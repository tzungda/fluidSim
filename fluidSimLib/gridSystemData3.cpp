//---gridSystemData3.cpp

#include "gridSystemData3.h"

gridSystemData3::gridSystemData3() :gridSystemData3(size3(0, 0, 0), vector3(1.0, 1.0, 1.0), vector3(0.0, 0.0, 0.0))
{
}

gridSystemData3::gridSystemData3(const size3& resolution, const vector3& gridSpacing, const vector3& origin)
{
    mVelocity = std::make_shared<faceCenteredGrid3>();
    mAdvectableVectorDataList.push_back(mVelocity);
    mVelocityIdx = 0;
    resize(resolution, gridSpacing, origin);
}

gridSystemData3::~gridSystemData3()
{
}

void gridSystemData3::resize(const size3& resolution, const vector3& gridSpacing, const vector3& origin)
{
    mResolution = resolution;
    mGridSpacing = gridSpacing;
    mOrigin = origin;

    for (scalarGrid3Ptr& data : mScalarGrid3DataList)
    {
        data->resize(resolution, gridSpacing, origin);
    }
    for (vectorGrid3Ptr& data : mVectorGrid3DataList)
    {
        data->resize(resolution, gridSpacing, origin);
    }
    for (scalarGrid3Ptr& data : mAdvectableScalarDataList)
    {
        data->resize(resolution, gridSpacing, origin);
    }
    for (vectorGrid3Ptr& data : mAdvectableVectorDataList)
    {
        data->resize(resolution, gridSpacing, origin);
    }
}

size3 gridSystemData3::resolution() const
{
    return mResolution;
}

vector3 gridSystemData3::gridSpacing() const
{
    return mGridSpacing;
}

vector3 gridSystemData3::origin() const
{
    return mOrigin;
}

boundingBox3 gridSystemData3::boundingBox() const
{
    return mVelocity->boundingBox();
}

size_t gridSystemData3::addScalarGrid3Data(const scalarGrid3Ptr scalarGrid3Data, FloatType initValue)
{
    size_t attrIdx = mScalarGrid3DataList.size();
    scalarGrid3Data->fill( initValue );
    mScalarGrid3DataList.push_back(scalarGrid3Data);

    return attrIdx;
}

size_t gridSystemData3::addVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue)
{
    size_t attrIdx = mVectorGrid3DataList.size();
    vectorGrid3Data->fill( initValue );
    mVectorGrid3DataList.push_back(vectorGrid3Data);

    return attrIdx;
}

size_t gridSystemData3::addAdvectableScalarData(const scalarGrid3Ptr& scalarGrid3Data, FloatType initialVal)
{
    size_t attrIdx = this->mAdvectableScalarDataList.size();
    scalarGrid3Data->fill( initialVal );
    mAdvectableScalarDataList.push_back(scalarGrid3Data);

    return attrIdx;
}

size_t gridSystemData3::addAdvectableVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue)
{
    size_t attrIdx = this->mAdvectableVectorDataList.size();
    vectorGrid3Data->fill( initValue );
    mAdvectableVectorDataList.push_back(vectorGrid3Data);

    return attrIdx;
}

const faceCenteredGrid3Ptr& gridSystemData3::velocity() const
{
    return mVelocity;
}

size_t gridSystemData3::velocityIndex() const
{
    return mVelocityIdx;
}

const scalarGrid3Ptr& gridSystemData3::scalarGrid3DataAt(size_t ind) const
{
    return mScalarGrid3DataList[ind];
}

const vectorGrid3Ptr& gridSystemData3::vectorGrid3DataAt(size_t ind) const
{
    return mVectorGrid3DataList[ind];
}

const scalarGrid3Ptr& gridSystemData3::advectableScalarDataAt(size_t idx) const
{
    return mAdvectableScalarDataList[idx];
}

const vectorGrid3Ptr& gridSystemData3::advectableVectorDataAt(size_t idx) const
{
    return mAdvectableVectorDataList[idx];
}

size_t gridSystemData3::numOfScalarGrid3Data() const
{
    return mScalarGrid3DataList.size();
}

size_t gridSystemData3::numOfVectorGrid3Data() const
{
    return mVectorGrid3DataList.size();
}

size_t gridSystemData3::numberOfAdvectableScalarData() const
{
    return mAdvectableScalarDataList.size();
}

size_t gridSystemData3::numberOfAdvectableVectorData() const
{
    return mAdvectableVectorDataList.size();
}


