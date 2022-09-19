//---particleSystemData3.cpp

#include <algorithm>

#include "particleSystemData3.h"
#include "pointHashGridSearcher3.h"

static const SizeType kDefaultHashGridResolution = 64;

particleSystemData3::particleSystemData3()
    : particleSystemData3(0) {
}

particleSystemData3::particleSystemData3(SizeType numberOfParticles) {
    mPositionIdx = addVectorData();
    mVelocityIdx = addVectorData();
    mForceIdx = addVectorData();

    // Use pointHashGridSearcher3 by default
    mNeighborSearcher = std::make_shared<pointHashGridSearcher3>(
        kDefaultHashGridResolution,
        kDefaultHashGridResolution,
        kDefaultHashGridResolution,
        (FloatType)2.0 * mRadius);

    resize(numberOfParticles);
}

particleSystemData3::particleSystemData3(const particleSystemData3& other) {
    set(other);
}

particleSystemData3::~particleSystemData3() {
}

void particleSystemData3::resize(SizeType newNumberOfParticles) {
    mNumberOfParticles = newNumberOfParticles;

    for (auto& attr : mScalarDataList) {
        attr.resize(newNumberOfParticles, 0.0);
    }

    for (auto& attr : mVectorDataList) {
        attr.resize(newNumberOfParticles, vector3());
    }
}

SizeType particleSystemData3::numberOfParticles() const {
    return mNumberOfParticles;
}

SizeType particleSystemData3::addScalarData(FloatType initialVal) {
    SizeType attrIdx = (SizeType)mScalarDataList.size();
    mScalarDataList.emplace_back(numberOfParticles(), initialVal);
    return attrIdx;
}

SizeType particleSystemData3::addVectorData(const vector3& initialVal) {
    SizeType attrIdx = (SizeType)mVectorDataList.size();
    mVectorDataList.emplace_back(numberOfParticles(), initialVal);
    return attrIdx;
}

FloatType particleSystemData3::radius() const {
    return mRadius;
}

void particleSystemData3::setRadius(FloatType newRadius) 
{
    mRadius = std::max<FloatType>(newRadius, (FloatType)0.0);
}

FloatType particleSystemData3::mass() const
{
    return mMass;
}

void particleSystemData3::setMass(FloatType newMass) 
{
    mMass = std::max<FloatType>(newMass, (FloatType)0.0);
}

const std::vector<vector3>& particleSystemData3::positions() const
{
    return vectorDataAt(mPositionIdx);
}

std::vector<vector3>& particleSystemData3::positions()
{
    return vectorDataAt(mPositionIdx);
}

const std::vector<vector3>& particleSystemData3::velocities() const
{
    return vectorDataAt(mVelocityIdx);
}

std::vector<vector3>& particleSystemData3::velocities()
{
    return vectorDataAt(mVelocityIdx);
}

const std::vector<vector3>& particleSystemData3::forces() const
{
    return vectorDataAt(mForceIdx);
}

std::vector<vector3>& particleSystemData3::forces()
{
    return vectorDataAt(mForceIdx);
}

const std::vector<FloatType>& particleSystemData3::scalarDataAt( SizeType idx) const
{
    return mScalarDataList[idx];
}

std::vector<FloatType>& particleSystemData3::scalarDataAt(SizeType idx)
{
    return mScalarDataList[idx];
}

const std::vector<vector3>& particleSystemData3::vectorDataAt( SizeType idx) const
{
    return mVectorDataList[idx];
}

std::vector<vector3>& particleSystemData3::vectorDataAt(SizeType idx)
{
    return mVectorDataList[idx];
}

void particleSystemData3::addParticle(
    const vector3& newPosition,
    const vector3& newVelocity,
    const vector3& newForce)
{
    std::vector<vector3> newPositions = {newPosition};
    std::vector<vector3> newVelocities = {newVelocity};
    std::vector<vector3> newForces = {newForce};

    addParticles( newPositions, newVelocities, newForces);
}

void particleSystemData3::addParticles(
    const std::vector<vector3>& newPositions,
    const std::vector<vector3>& newVelocities,
    const std::vector<vector3>& newForces)
{


    SizeType oldNumberOfParticles = numberOfParticles();
    SizeType newNumberOfParticles = oldNumberOfParticles + (SizeType)newPositions.size();

    resize(newNumberOfParticles);

    std::vector<vector3>& pos = positions();
    std::vector<vector3>& vel = velocities();
    std::vector<vector3>& frc = forces();

    for ( SizeType i = 0; i < newPositions.size(); ++i )
    {
        pos[i + oldNumberOfParticles] = newPositions[i];
    }


    if (newVelocities.size() > 0)
    {
        for ( SizeType i = 0; i < newPositions.size(); ++i )
        {
            vel[i + oldNumberOfParticles] = newVelocities[i];
        }
    }

    if (newForces.size() > 0)
    {
        for ( SizeType i = 0; i < newPositions.size(); ++i )
        {
            frc[i + oldNumberOfParticles] = newForces[i];
        }
    }
}

const pointNeighborSearcher3Ptr& particleSystemData3::neighborSearcher() const {
    return mNeighborSearcher;
}

void particleSystemData3::setNeighborSearcher(
    const pointNeighborSearcher3Ptr& newNeighborSearcher) {
    mNeighborSearcher = newNeighborSearcher;
}

const std::vector<std::vector<SizeType>>&
particleSystemData3::neighborLists() const {
    return mNeighborLists;
}

void particleSystemData3::buildNeighborSearcher(FloatType maxSearchRadius)
{
    // Use pointHashGridSearcher3 by default
    mNeighborSearcher = std::make_shared<pointHashGridSearcher3>(
        kDefaultHashGridResolution,
        kDefaultHashGridResolution,
        kDefaultHashGridResolution,
        (FloatType)2.0 * maxSearchRadius);

    mNeighborSearcher->build(positions());

}

void particleSystemData3::buildNeighborLists(FloatType maxSearchRadius)
{

    mNeighborLists.resize(numberOfParticles());

    auto points = positions();
    for (SizeType i = 0; i < numberOfParticles(); ++i) {
        vector3 origin = points[i];
        mNeighborLists[i].clear();

        mNeighborSearcher->forEachNearbyPoint(
            origin,
            maxSearchRadius,
            [&](SizeType j, const vector3&) {
            if (i != j) {
                mNeighborLists[i].push_back(j);
            }
        });
    }

}


void particleSystemData3::set(const particleSystemData3& other) {
    mRadius = other.mRadius;
    mMass = other.mMass;
    mPositionIdx = other.mPositionIdx;
    mVelocityIdx = other.mVelocityIdx;
    mForceIdx = other.mForceIdx;
    mNumberOfParticles = other.mNumberOfParticles;

    for (auto& attr : other.mScalarDataList) {
        mScalarDataList.emplace_back(attr);
    }

    for (auto& attr : other.mVectorDataList) {
        mVectorDataList.emplace_back(attr);
    }

    mNeighborSearcher = other.mNeighborSearcher->clone();
    mNeighborLists = other.mNeighborLists;
}

particleSystemData3& particleSystemData3::operator=(
    const particleSystemData3& other) {
    set(other);
    return *this;
}

