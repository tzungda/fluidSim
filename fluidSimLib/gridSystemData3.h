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
    size_t mVelocityIdx;
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

    size_t addScalarGrid3Data(const scalarGrid3Ptr scalarGrid3Data, double initValue = 0.0);
    size_t addVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue = vector3());
    size_t addAdvectableScalarData(const scalarGrid3Ptr& scalarGrid3Data, double initialVal = 0.0);
    size_t addAdvectableVectorGrid3Data(const vectorGrid3Ptr vectorGrid3Data, const vector3& initValue = vector3());

    const faceCenteredGrid3Ptr& velocity() const;
    size_t velocityIndex() const;

    const scalarGrid3Ptr& scalarGrid3DataAt(size_t ind) const;
    const vectorGrid3Ptr& vectorGrid3DataAt(size_t ind) const;
    const scalarGrid3Ptr& advectableScalarDataAt(size_t idx) const;
    const vectorGrid3Ptr& advectableVectorDataAt(size_t idx) const;

    size_t numOfScalarGrid3Data() const;
    size_t numOfVectorGrid3Data() const;
    size_t numberOfAdvectableScalarData() const;
    size_t numberOfAdvectableVectorData() const;
};

typedef std::shared_ptr<gridSystemData3> gridSystemData3Ptr;

#endif

