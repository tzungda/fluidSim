//---gridFractionalSinglePhasePressureSolver3.h
#ifndef gridFractionalSinglePhasePressureSolver3_H
#define gridFractionalSinglePhasePressureSolver3_H


#include "gridPressureSolver3.h"
#include "gridBoundaryConditionSolver3.h"
#include "fdmLinearSystemSolver3.h"
#include "cellCenteredScalarGrid3.h"


class gridFractionalSinglePhasePressureSolver3 : public gridPressureSolver3
{
public:
    gridFractionalSinglePhasePressureSolver3();

    virtual ~gridFractionalSinglePhasePressureSolver3();

   
    void solve(
        const faceCenteredGrid3& input,
        FloatType timeIntervalInSeconds,
        faceCenteredGrid3* output,
        const scalarField3& boundarySdf
        = constantScalarField3(mathUtil::maxFloat()),
        const vectorField3& boundaryVelocity
        = constantVectorField3({0, 0, 0}),
        const scalarField3& fluidSdf
        = constantScalarField3(-mathUtil::maxFloat())) override;

    gridBoundaryConditionSolver3Ptr
        suggestedBoundaryConditionSolver() const override;

    void setLinearSystemSolver(const fdmLinearSystemSolver3Ptr& solver);

    const dataBuffer3& pressure() const;

private:
    fdmLinearSystem3 mSystem;
    fdmLinearSystemSolver3Ptr mSystemSolver;
    dataBuffer3 mWeightsU;
    dataBuffer3 mWeightsV;
    dataBuffer3 mWeightsW;
    dataBuffer3 mBoundaryU;
    dataBuffer3 mBoundaryV;
    dataBuffer3 mBoundaryW;
    cellCenteredScalarGrid3 mFluidSdf;

    void buildWeights(
        const faceCenteredGrid3& input,
        const scalarField3& boundarySdf,
        const vectorField3& boundaryVelocity,
        const scalarField3& fluidSdf);

    virtual void buildSystem(const faceCenteredGrid3& input);

    virtual void applyPressureGradient(
        const faceCenteredGrid3& input,
        faceCenteredGrid3* output);
};

typedef std::shared_ptr<gridFractionalSinglePhasePressureSolver3> gridFractionalSinglePhasePressureSolver3Ptr;

#endif

