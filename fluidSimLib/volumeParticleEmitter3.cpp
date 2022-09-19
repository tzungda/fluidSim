
//---volumeParticleEmitter3.cpp

#include "volumeParticleEmitter3.h"
#include "bccLatticePointGenerator.h"
#include "pointHashGridSearcher3.h"
#include "mathUtil.h"

static const size_t kDefaultHashGridResolution = 64;

volumeParticleEmitter3::volumeParticleEmitter3(
    const implicitSurface3Ptr& implicitSurface,
    const boundingBox3& bounds,
    FloatType spacing,
    const vector3& initialVel,
    size_t maxNumberOfParticles,
    FloatType jitter,
    bool isOneShot,
    bool allowOverlapping,
    uint32_t seed) :
    mRng(seed),
    mImplicitSurface(implicitSurface),
    mBounds(bounds),
    mSpacing(spacing),
    mInitialVel(initialVel),
    mMaxNumberOfParticles(maxNumberOfParticles),
    mJitter(jitter),
    mIsOneShot(isOneShot),
    mAllowOverlapping(allowOverlapping)
{
    mPointsGen = std::make_shared<bccLatticePointGenerator>();
}

void volumeParticleEmitter3::onUpdate(
    FloatType currentTimeInSeconds,
    FloatType timeIntervalInSeconds)
{

    auto particles = target();

    if (particles == nullptr) {
        return;
    }

    if (mNumberOfEmittedParticles > 0 && mIsOneShot) {
        return;
    }

    std::vector<vector3> newPositions;
    std::vector<vector3> newVelocities;

    emit(particles, &newPositions, &newVelocities);

    particles->addParticles(newPositions, newVelocities);
}

void volumeParticleEmitter3::emit(
    const particleSystemData3Ptr& particles,
    std::vector<vector3>* newPositions,
    std::vector<vector3>* newVelocities)
{
    // Reserving more space for jittering
    const FloatType j = jitter();
    const FloatType maxJitterDist = (FloatType)0.5 * j * mSpacing;

    if (mAllowOverlapping || mIsOneShot) {
        mPointsGen->forEachPoint(
            mBounds,
            mSpacing,
            [&] (const vector3& point) {
                vector3 randomDir = mathUtil::uniformSampleSphere(
                    random(),
                    random());
                vector3 offset = maxJitterDist * randomDir;
                vector3 candidate = point + offset;
                if (mImplicitSurface->signedDistance(candidate) <= 0.0) {
                    if (mNumberOfEmittedParticles < mMaxNumberOfParticles) {
                        newPositions->push_back(candidate);
                        ++mNumberOfEmittedParticles;
                    } else {
                        return false;
                    }
                }

                return true;
            });
    } else {
        // Use serial hash grid searcher for continuous update.
        pointHashGridSearcher3 neighborSearcher(
            size3(
                kDefaultHashGridResolution,
                kDefaultHashGridResolution,
                kDefaultHashGridResolution),
            (FloatType)2.0 * mSpacing);
        if (!mAllowOverlapping) {
            neighborSearcher.build(particles->positions());
        }

        mPointsGen->forEachPoint(
            mBounds,
            mSpacing,
            [&] (const vector3& point) {
                vector3 randomDir = mathUtil::uniformSampleSphere(
                    random(),
                    random());
                vector3 offset = maxJitterDist * randomDir;
                vector3 candidate = point + offset;
                if (mImplicitSurface->signedDistance(candidate) <= 0.0 &&
                    (!mAllowOverlapping &&
                        !neighborSearcher.hasNearbyPoint(candidate, mSpacing))) {
                    if (mNumberOfEmittedParticles < mMaxNumberOfParticles) {
                        newPositions->push_back(candidate);
                        neighborSearcher.add(candidate);
                        ++mNumberOfEmittedParticles;
                    } else {
                        return false;
                    }
                }

                return true;
            });
    }

    newVelocities->resize(newPositions->size(), mInitialVel);
    //newVelocities->set(mInitialVel);
}

void volumeParticleEmitter3::setPointGenerator( const pointGenerator3Ptr& newPointsGen)
{
    mPointsGen = newPointsGen;
}

FloatType volumeParticleEmitter3::jitter() const
{
    return mJitter;
}

void volumeParticleEmitter3::setJitter(FloatType newJitter)
{
    mJitter = mathUtil::clamp(newJitter, 0.0, 1.0);
}

bool volumeParticleEmitter3::isOneShot() const
{
    return mIsOneShot;
}

void volumeParticleEmitter3::setIsOneShot(bool newValue)
{
    mIsOneShot = newValue;
}

bool volumeParticleEmitter3::allowOverlapping() const
{
    return mAllowOverlapping;
}

void volumeParticleEmitter3::setAllowOverlapping(bool newValue)
{
    mAllowOverlapping = newValue;
}

size_t volumeParticleEmitter3::maxNumberOfParticles() const
{
    return mMaxNumberOfParticles;
}

void volumeParticleEmitter3::setMaxNumberOfParticles( size_t newMaxNumberOfParticles )
{
    mMaxNumberOfParticles = newMaxNumberOfParticles;
}

FloatType volumeParticleEmitter3::spacing() const
{
    return mSpacing;
}

void volumeParticleEmitter3::setSpacing(FloatType newSpacing)
{
    mSpacing = newSpacing;
}

vector3 volumeParticleEmitter3::initialVelocity() const
{
    return mInitialVel;
}

void volumeParticleEmitter3::setInitialVelocity(const vector3& newInitialVel)
{
    mInitialVel = newInitialVel;
}

FloatType volumeParticleEmitter3::random()
{
    std::uniform_real_distribution<> d(0.0, 1.0);
    return (FloatType)d(mRng);
}