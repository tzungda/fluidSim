//---particleEmitter3.cpp

#include "particleEmitter3.h"


particleEmitter3::particleEmitter3()
{
}

particleEmitter3::~particleEmitter3()
{
}

const particleSystemData3Ptr& particleEmitter3::target() const
{
    return mParticles;
}

void particleEmitter3::setTarget(const particleSystemData3Ptr& particles)
{
    mParticles = particles;

    onSetTarget(particles);
}

void particleEmitter3::update(
    FloatType currentTimeInSeconds,
    FloatType timeIntervalInSeconds)
{
    if (mOnBeginUpdateCallback)
    {
        mOnBeginUpdateCallback(
            this, currentTimeInSeconds, timeIntervalInSeconds);
    }

    onUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

void particleEmitter3::onSetTarget(const particleSystemData3Ptr& particles)
{
}

void particleEmitter3::setOnBeginUpdateCallback(
    const OnBeginUpdateCallback& callback)
{
    mOnBeginUpdateCallback = callback;
}