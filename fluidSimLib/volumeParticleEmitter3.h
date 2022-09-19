

//---volumeParticleEmitter3.h
#ifndef volumeParticleEmitter3_H
#define volumeParticleEmitter3_H

#include <random>

#include "particleEmitter3.h"
#include "implicitSurface3.h"
#include "boundingBox3.h"
#include "pointGenerator3.h"

class volumeParticleEmitter3 : public particleEmitter3
{
public:
    volumeParticleEmitter3(
        const implicitSurface3Ptr& implicitSurface,
        const boundingBox3& bounds,
        FloatType spacing,
        const vector3& initialVel = vector3(),
        SizeType maxNumberOfParticles = std::numeric_limits<SizeType>::max(),
        FloatType jitter = 0.0,
        bool isOneShot = true,
        bool allowOverlapping = false,
        uint32_t seed = 0);


    void setPointGenerator(const pointGenerator3Ptr& newPointsGen);

    FloatType jitter() const;

    void setJitter(FloatType newJitter);

    bool isOneShot() const;

    void setIsOneShot(bool newValue);

    bool allowOverlapping() const;

    void setAllowOverlapping(bool newValue);

    SizeType maxNumberOfParticles() const;

    void setMaxNumberOfParticles(SizeType newMaxNumberOfParticles);

    FloatType spacing() const;

    //! Sets the spacing between particles.
    void setSpacing(FloatType newSpacing);

    //! Sets the initial velocity of the particles.
    vector3 initialVelocity() const;

    //! Returns the initial velocity of the particles.
    void setInitialVelocity(const vector3& newInitialVel);


private:
    std::mt19937 mRng;

    implicitSurface3Ptr mImplicitSurface;
    boundingBox3 mBounds;
    FloatType mSpacing;
    vector3 mInitialVel;
    pointGenerator3Ptr mPointsGen;

    SizeType mMaxNumberOfParticles = std::numeric_limits<SizeType>::max();
    SizeType mNumberOfEmittedParticles = 0;

    FloatType mJitter = 0.0;
    bool mIsOneShot = true;
    bool mAllowOverlapping = false;

    //!
    //! \brief      Emits particles to the particle system data.
    //!
    //! \param[in]  currentTimeInSeconds    Current simulation time.
    //! \param[in]  timeIntervalInSeconds   The time-step interval.
    //!
    void onUpdate(
        FloatType currentTimeInSeconds,
        FloatType timeIntervalInSeconds) override;

    void emit(
        const particleSystemData3Ptr& particles,
        std::vector<vector3>* newPositions,
        std::vector<vector3>* newVelocities);

    FloatType random();
};

typedef std::shared_ptr<volumeParticleEmitter3> volumeParticleEmitter3Ptr;

#endif