//---gridPressureSolver3.h
#ifndef gridPressureSolver3_H
#define gridPressureSolver3_H

#include "faceCenteredGrid3.h"
#include "scalarGrid3.h"
#include "mathUtil.h"
#include "gridBoundaryConditionSolver3.h"

class gridPressureSolver3
{
public:
    //! Default constructor.
    gridPressureSolver3();

    //! Default destructor.
    virtual ~gridPressureSolver3();

    virtual void solve(
        const faceCenteredGrid3& input,
        double timeIntervalInSeconds,
        faceCenteredGrid3* output,
        const scalarField3& boundarySdf
        = constantScalarField3(mathUtil::maxFloat()),
        const vectorField3& boundaryVelocity
        = constantVectorField3({0, 0, 0}),
        const scalarField3& fluidSdf
        = constantScalarField3(-mathUtil::maxFloat())) = 0;


    virtual gridBoundaryConditionSolver3Ptr
        suggestedBoundaryConditionSolver() const = 0;
};

typedef std::shared_ptr<gridPressureSolver3> gridPressureSolver3Ptr;

#endif

