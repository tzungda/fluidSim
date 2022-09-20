//---particleEmitterSet3.h
#ifndef particleEmitterSet3_H
#define particleEmitterSet3_H

#include "particleEmitter3.h"

class particleEmitterSet3 : public particleEmitter3
{
public:
   
    particleEmitterSet3();

    explicit particleEmitterSet3(
        const std::vector<particleEmitter3Ptr>& emitters);

    virtual ~particleEmitterSet3();

    //! Adds sub-emitter.
    void addEmitter(const particleEmitter3Ptr& emitter);


private:
    std::vector<particleEmitter3Ptr> mEmitters;

    void onSetTarget(const particleSystemData3Ptr& particles) override;

    void onUpdate(
        double currentTimeInSeconds,
        double timeIntervalInSecond) override;
};

typedef std::shared_ptr< particleEmitterSet3 > particleEmitterSet3Ptr;

#endif

