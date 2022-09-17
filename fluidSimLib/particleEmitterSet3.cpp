//---particleEmitterSet3.cpp

#include "particleEmitterSet3.h"

particleEmitterSet3::particleEmitterSet3()
{
}

particleEmitterSet3::particleEmitterSet3( const std::vector<particleEmitter3Ptr>& emitters)
    : mEmitters(emitters)
{
}

particleEmitterSet3::~particleEmitterSet3()
{
}

void particleEmitterSet3::addEmitter(const particleEmitter3Ptr& emitter)
{
    mEmitters.push_back(emitter);
}

void particleEmitterSet3::onSetTarget(const particleSystemData3Ptr& particles)
{
    for (auto& emitter : mEmitters) {
        emitter->setTarget(particles);
    }
}

void particleEmitterSet3::onUpdate(
    double currentTimeInSeconds,
    double timeIntervalInSeconds)
{
    for (auto& emitter : mEmitters)
    {
        emitter->update(currentTimeInSeconds, timeIntervalInSeconds);
    }
}


