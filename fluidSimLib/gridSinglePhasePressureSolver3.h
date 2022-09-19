//---gridSinglePhasePressureSolver3.h
#ifndef gridSinglePhasePressureSolver3_H
#define gridSinglePhasePressureSolver3_H

#include "gridPressureSolver3.h"
#include "markers3.h"
#include "fdmLinearSystemSolver3.h"

class gridSinglePhasePressureSolver3 : public gridPressureSolver3
{
public:
    //! Default constructor.
    gridSinglePhasePressureSolver3();

    //! Default destructor.
    virtual ~gridSinglePhasePressureSolver3();

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

    //! Sets the linear system solver.
    void setLinearSystemSolver(const fdmLinearSystemSolver3Ptr& solver);

    //! Returns the pressure field.
    const dataBuffer3& pressure() const;

private:
    fdmLinearSystem3 mSystem;
    fdmLinearSystemSolver3Ptr mSystemSolver;
    //Array3<char> _markers;
    markers3 mMarkers;

    void buildMarkers(
        const size3& size,
        const std::function<vector3(SizeType, SizeType, SizeType)>& pos,
        const scalarField3& boundarySdf,
        const scalarField3& fluidSdf);

    virtual void buildSystem(const faceCenteredGrid3& input);

    virtual void applyPressureGradient(
        const faceCenteredGrid3& input,
        faceCenteredGrid3* output);
};

typedef std::shared_ptr<gridPressureSolver3> gridPressureSolver3Ptr;

#endif

