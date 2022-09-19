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

SizeType gridSystemData3::addScalarGrid3Data(const scalarGrid3Ptr scalarGrid3Data, FloatType initValue)
{
    SizeType attrIdx = (SizeType)mScalarGrid3DataList.size();
    scalarGrid3Data->fill( initValue );
    mScalarGrid3DataList.push_back(scalarGrid3Data);

    return attrIdx;
}

SizeType gridSystemData3::addVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue)
{
    SizeType attrIdx = (SizeType)mVectorGrid3DataList.size();
    vectorGrid3Data->fill( initValue );
    mVectorGrid3DataList.push_back(vectorGrid3Data);

    return attrIdx;
}

SizeType gridSystemData3::addAdvectableScalarData(const scalarGrid3Ptr& scalarGrid3Data, FloatType initialVal)
{
    SizeType attrIdx = (SizeType)this->mAdvectableScalarDataList.size();
    scalarGrid3Data->fill( initialVal );
    mAdvectableScalarDataList.push_back(scalarGrid3Data);

    return attrIdx;
}

SizeType gridSystemData3::addAdvectableVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue)
{
    SizeType attrIdx = (SizeType)this->mAdvectableVectorDataList.size();
    vectorGrid3Data->fill( initValue );
    mAdvectableVectorDataList.push_back(vectorGrid3Data);

    return attrIdx;
}

const faceCenteredGrid3Ptr& gridSystemData3::velocity() const
{
    return mVelocity;
}

SizeType gridSystemData3::velocityIndex() const
{
    return mVelocityIdx;
}

const scalarGrid3Ptr& gridSystemData3::scalarGrid3DataAt(SizeType ind) const
{
    return mScalarGrid3DataList[ind];
}

const vectorGrid3Ptr& gridSystemData3::vectorGrid3DataAt(SizeType ind) const
{
    return mVectorGrid3DataList[ind];
}

const scalarGrid3Ptr& gridSystemData3::advectableScalarDataAt(SizeType idx) const
{
    return mAdvectableScalarDataList[idx];
}

const vectorGrid3Ptr& gridSystemData3::advectableVectorDataAt(SizeType idx) const
{
    return mAdvectableVectorDataList[idx];
}

SizeType gridSystemData3::numOfScalarGrid3Data() const
{
    return (SizeType)mScalarGrid3DataList.size();
}

SizeType gridSystemData3::numOfVectorGrid3Data() const
{
    return (SizeType)mVectorGrid3DataList.size();
}

SizeType gridSystemData3::numberOfAdvectableScalarData() const
{
    return (SizeType)mAdvectableScalarDataList.size();
}

SizeType gridSystemData3::numberOfAdvectableVectorData() const
{
    return (SizeType)mAdvectableVectorDataList.size();
}


