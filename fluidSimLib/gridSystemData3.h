//---gridSystemData3.h
#ifndef gridSystemData3_H
#define gridSystemData3_H

#include "faceCenteredGrid3.h"
#include "scalarGrid3.h"
#include "vectorGrid3.h"

class gridSystemData3
{
private:
    size3 mResolution;
    vector3 mGridSpacing;
    vector3 mOrigin;
    //
    faceCenteredGrid3Ptr mVelocity;
    SizeType mVelocityIdx;
    std::vector<scalarGrid3Ptr> mScalarGrid3DataList;
    std::vector<vectorGrid3Ptr> mVectorGrid3DataList;
    std::vector<scalarGrid3Ptr> mAdvectableScalarDataList;
    std::vector<vectorGrid3Ptr> mAdvectableVectorDataList;

public:
    gridSystemData3();
    gridSystemData3(const size3& resolution, const vector3& gridSpacing, const vector3& origin);
    virtual ~gridSystemData3();

    void resize(const size3& resolution, const vector3& gridSpacing, const vector3& origin);

    size3 resolution() const;

    vector3 gridSpacing() const;

    vector3 origin() const;

    boundingBox3 boundingBox() const;

    SizeType addScalarGrid3Data(const scalarGrid3Ptr scalarGrid3Data, FloatType initValue = 0.0);
    SizeType addVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue = vector3());
    SizeType addAdvectableScalarData(const scalarGrid3Ptr& scalarGrid3Data, FloatType initialVal = 0.0);
    SizeType addAdvectableVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue = vector3());

    const faceCenteredGrid3Ptr& velocity() const;
    SizeType velocityIndex() const;

    const scalarGrid3Ptr& scalarGrid3DataAt(SizeType ind) const;
    const vectorGrid3Ptr& vectorGrid3DataAt(SizeType ind) const;
    const scalarGrid3Ptr& advectableScalarDataAt(SizeType idx) const;
    const vectorGrid3Ptr& advectableVectorDataAt(SizeType idx) const;

    SizeType numOfScalarGrid3Data() const;
    SizeType numOfVectorGrid3Data() const;
    SizeType numberOfAdvectableScalarData() const;
    SizeType numberOfAdvectableVectorData() const;
};

typedef std::shared_ptr<gridSystemData3> gridSystemData3Ptr;

#endif

