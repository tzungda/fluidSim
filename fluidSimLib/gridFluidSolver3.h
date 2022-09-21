//---gridFluidSolver3.h
#ifndef gridFluidSolver3_H
#define gridFluidSolver3_H

#include "simAnimation.h"
#include "gridSystemData3.h"
#include "advectionSolver3.h"
#include "cellCenteredScalarGrid3.h"
#include "cellCenteredVectorGrid3.h"
#include "gridBoundaryConditionSolver3.h"
#include "gridDiffusionSolver3.h"
#include "gridPressureSolver3.h"
#include "gridEmitter3.h"
#include "boundaryDef.h"

class gridFluidSolver3 : public simAnimation
{
public:
    gridFluidSolver3();
    gridFluidSolver3(
        const size3& resolution,
        const vector3& gridSpacing,
        const vector3& gridOrigin);
    virtual ~gridFluidSolver3();

    const vector3& gravity() const;
    const faceCenteredGrid3Ptr& velocity() const;

    const advectionSolver3Ptr& advectionSolver() const;
    void setAdvectionSolver(const advectionSolver3Ptr& newAdvectSolver);
    const gridDiffusionSolver3Ptr& diffusionSolver() const;
    void setDiffusionSolver(const gridDiffusionSolver3Ptr& newDiffusionSolver);
    const gridPressureSolver3Ptr& pressureSolver() const;
    void setPressureSolver(const gridPressureSolver3Ptr& newPressureSolver);

    void setEmitter(const gridEmitter3Ptr& newEmitter);

    int closedDomainBoundaryFlag() const;
    FloatType cfl(FloatType timeIntervalInSeconds) const;
    FloatType maxCfl() const;

    collider3Ptr collider() const;

    const cellCenteredScalarGrid3& colliderSdf() const;

    void computeGravity(FloatType timeIntervalInSeconds);

    void resizeGrid(
        const size3& newSize,
        const vector3& newGridSpacing,
        const vector3& newGridOrigin);

    const gridSystemData3Ptr& gridSystemData() const;

protected:
    void onInitialize() override;
    void onAdvanceTimeStep(FloatType timeIntervalInSeconds) override;

    unsigned int numberOfSubTimeSteps(
        FloatType timeIntervalInSeconds) const override;

    virtual void onBeginAdvanceTimeStep(FloatType timeIntervalInSeconds);
    virtual void onEndAdvanceTimeStep(FloatType timeIntervalInSeconds);
    virtual scalarField3Ptr fluidSdf() const;

    virtual void computeExternalForces(FloatType timeIntervalInSeconds);
    virtual void computeViscosity(FloatType timeIntervalInSeconds);
    virtual void computePressure(FloatType timeIntervalInSeconds);
    virtual void computeAdvection(FloatType timeIntervalInSeconds);


    void extrapolateIntoCollider(scalarGrid3* grid);
    void extrapolateIntoCollider(extendVectorGrid3* grid);
    void extrapolateIntoCollider(faceCenteredGrid3* grid);
    void applyBoundaryCondition();

private:
    void beginAdvanceTimeStep(FloatType timeIntervalInSeconds);
    void endAdvanceTimeStep(FloatType timeIntervalInSeconds);

    void updateCollider(FloatType timeIntervalInSeconds);
    void updateEmitter(FloatType timeIntervalInSeconds);

private:
    vector3 mGravity = vector3(0.0, (FloatType)-9.8, 0.0);
    FloatType mViscosityCoefficient = 0.0;
    FloatType mMaxCfl = (FloatType)5.0;
    int mClosedDomainBoundaryFlag = kDirectionAll;
    //bool isDirectionBoundary = true;

    gridSystemData3Ptr mGrids;
    collider3Ptr mCollider;
    gridEmitter3Ptr mEmitter;
    advectionSolver3Ptr mAdvectionSolver;
    cellCenteredScalarGrid3 mColliderSdf;
    cellCenteredVectorGrid3 mColliderVel;
    gridBoundaryConditionSolver3Ptr mBoundaryConditionSolver;
    gridDiffusionSolver3Ptr mDiffusionSolver;
    gridPressureSolver3Ptr mPressureSolver;

};

#endif

