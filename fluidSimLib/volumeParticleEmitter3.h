

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
        double spacing,
        const vector3& initialVel = vector3(),
        size_t maxNumberOfParticles = std::numeric_limits<size_t>::max(),
        double jitter = 0.0,
        bool isOneShot = true,
        bool allowOverlapping = false,
        uint32_t seed = 0);


    void setPointGenerator(const pointGenerator3Ptr& newPointsGen);

    double jitter() const;

    void setJitter(double newJitter);

    bool isOneShot() const;

    void setIsOneShot(bool newValue);

    bool allowOverlapping() const;

    void setAllowOverlapping(bool newValue);

    size_t maxNumberOfParticles() const;

    void setMaxNumberOfParticles(size_t newMaxNumberOfParticles);

    double spacing() const;

    //! Sets the spacing between particles.
    void setSpacing(double newSpacing);

    //! Sets the initial velocity of the particles.
    vector3 initialVelocity() const;

    //! Returns the initial velocity of the particles.
    void setInitialVelocity(const vector3& newInitialVel);


private:
    std::mt19937 mRng;

    implicitSurface3Ptr mImplicitSurface;
    boundingBox3 mBounds;
    double mSpacing;
    vector3 mInitialVel;
    pointGenerator3Ptr mPointsGen;

    size_t mMaxNumberOfParticles = std::numeric_limits<size_t>::max();
    size_t mNumberOfEmittedParticles = 0;

    double mJitter = 0.0;
    bool mIsOneShot = true;
    bool mAllowOverlapping = false;

    //!
    //! \brief      Emits particles to the particle system data.
    //!
    //! \param[in]  currentTimeInSeconds    Current simulation time.
    //! \param[in]  timeIntervalInSeconds   The time-step interval.
    //!
    void onUpdate(
        double currentTimeInSeconds,
        double timeIntervalInSeconds) override;

    void emit(
        const particleSystemData3Ptr& particles,
        std::vector<vector3>* newPositions,
        std::vector<vector3>* newVelocities);

    double random();
};

typedef std::shared_ptr<volumeParticleEmitter3> volumeParticleEmitter3Ptr;

#endif