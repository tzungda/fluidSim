//---picSolver3.h
#ifndef picSolver3_H
#define picSolver3_H

#include "gridFluidSolver3.h"
#include "particleSystemData3.h"
#include "particleEmitter3.h"

class picSolver3 : public gridFluidSolver3
{
public:
    picSolver3();

    //! Constructs solver with initial grid size.
    picSolver3(
        const size3& resolution,
        const vector3& gridSpacing,
        const vector3& gridOrigin);

    //! Default destructor.
    virtual ~picSolver3();

    //! Returns the signed-distance field of particles.
    scalarGrid3Ptr signedDistanceField() const;

    //! Returns the particle system data.
    const particleSystemData3Ptr& particleSystemData() const;

    //! Returns the particle emitter.
    const particleEmitter3Ptr& particleEmitter() const;

    //! Sets the particle emitter.
    void setParticleEmitter(const particleEmitter3Ptr& newEmitter);


protected:
    //! Initializes the simulator.
    void onInitialize() override;

    //! Invoked before a simulation time-step begins.
    void onBeginAdvanceTimeStep(FloatType timeIntervalInSeconds) override;

    //! Computes the advection term of the fluid solver.
    void computeAdvection(FloatType timeIntervalInSeconds) override;

    //! Returns the signed-distance field of the fluid.
    scalarField3Ptr fluidSdf() const override;

    //! Transfers velocity field from particles to grids.
    virtual void transferFromParticlesToGrids();

    //! Transfers velocity field from grids to particles.
    virtual void transferFromGridsToParticles();

    //! Moves particles.
    virtual void moveParticles(FloatType timeIntervalInSeconds);

private:
    SizeType mSignedDistanceFieldId;
    particleSystemData3Ptr mParticles;
    particleEmitter3Ptr mParticleEmitter;

    markers3 mMarkersU;
    markers3 mMarkersV;
    markers3 mMarkersW;

    void extrapolateVelocityToAir();

    void buildSignedDistanceField();

    void updateParticleEmitter(FloatType timeIntervalInSeconds);
};

typedef std::shared_ptr<picSolver3> picSolver3Ptr;

#endif