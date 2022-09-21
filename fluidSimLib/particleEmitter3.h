//---particleEmitter3.h
#ifndef particleEmitter3_H
#define particleEmitter3_H

#include <memory>
#include <functional>

#include "particleSystemData3.h"

class particleEmitter3
{
public:

    typedef std::function<void(particleEmitter3*, FloatType, FloatType)>
        OnBeginUpdateCallback;

    particleEmitter3();

    virtual ~particleEmitter3();

    void update(FloatType currentTimeInSeconds, FloatType timeIntervalInSeconds);

    const particleSystemData3Ptr& target() const;

    void setTarget(const particleSystemData3Ptr& particles);

    void setOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

protected:

    virtual void onSetTarget(const particleSystemData3Ptr& particles);

    virtual void onUpdate(
        FloatType currentTimeInSeconds,
        FloatType timeIntervalInSeconds) = 0;

private:
    particleSystemData3Ptr mParticles;
    OnBeginUpdateCallback mOnBeginUpdateCallback;
};

typedef std::shared_ptr<particleEmitter3> particleEmitter3Ptr;

#endif
