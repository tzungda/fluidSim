//---particleSystemData3.h
#ifndef particleSystemData3_H
#define particleSystemData3_H

#include <memory>
#include <vector>

#include "vector3.h"
#include "pointNeighborSearcher3.h"

class particleSystemData3
{
public:
    typedef std::vector<FloatType> ScalarData;
    typedef std::vector<vector3> VectorData;

   
    particleSystemData3();

    explicit particleSystemData3(SizeType numberOfParticles);

    particleSystemData3(const particleSystemData3& other);

    virtual ~particleSystemData3();

    void resize(SizeType newNumberOfParticles);

    //! Returns the number of particles.
    SizeType numberOfParticles() const;

    SizeType addScalarData(FloatType initialVal = 0.0);

    SizeType addVectorData(const vector3& initialVal = vector3());

    FloatType radius() const;

    virtual void setRadius(FloatType newRadius);

    FloatType mass() const;

    virtual void setMass(FloatType newMass);

    const std::vector<vector3>& positions() const;

    std::vector<vector3>& positions();

    const std::vector<vector3>& velocities() const;

    std::vector<vector3>& velocities();

    const std::vector<vector3>& forces() const;

    std::vector<vector3>& forces();

    const std::vector<FloatType>& scalarDataAt(SizeType idx) const;

    std::vector<FloatType>& scalarDataAt(SizeType idx);

    const std::vector<vector3>& vectorDataAt(SizeType idx) const;

    std::vector<vector3>& vectorDataAt(SizeType idx);

    void addParticle(
        const vector3& newPosition,
        const vector3& newVelocity = vector3(),
        const vector3& newForce = vector3());

   
    void addParticles(
        const std::vector<vector3>& newPositions,
        const std::vector<vector3>& newVelocities
        = std::vector<vector3>(),
        const std::vector<vector3>& newForces
        = std::vector<vector3>());

    const pointNeighborSearcher3Ptr& neighborSearcher() const;


    void setNeighborSearcher(
        const pointNeighborSearcher3Ptr& newNeighborSearcher);


    const std::vector<std::vector<SizeType>>& neighborLists() const;


    void buildNeighborSearcher(FloatType maxSearchRadius);

    //! Builds neighbor lists with given search radius.
    void buildNeighborLists(FloatType maxSearchRadius);

    void set(const particleSystemData3& other);

    particleSystemData3& operator=(const particleSystemData3& other);



private:
    FloatType mRadius = (FloatType)1e-3;
    FloatType mMass = (FloatType)1e-3;
    SizeType mNumberOfParticles = 0;
    SizeType mPositionIdx;
    SizeType mVelocityIdx;
    SizeType mForceIdx;

    std::vector<ScalarData> mScalarDataList;
    std::vector<VectorData> mVectorDataList;

    pointNeighborSearcher3Ptr mNeighborSearcher;
    std::vector<std::vector<SizeType>> mNeighborLists;
};

typedef std::shared_ptr<particleSystemData3> particleSystemData3Ptr;


#endif
