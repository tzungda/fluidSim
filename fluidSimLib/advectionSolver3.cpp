//--- advectionSolver3.cpp

#include "advectionSolver3.h"


advectionSolver3::advectionSolver3()
{
}

advectionSolver3::~advectionSolver3()
{
}

void advectionSolver3::advect(
    const scalarGrid3& input,
    const vectorField3& flow,
    FloatType dt,
    scalarGrid3* output,
    const scalarField3& boundarySdf )
{
}

void advectionSolver3::advect(
    const extendVectorGrid3& input,
    const vectorField3& flow,
    FloatType dt,
    extendVectorGrid3* output,
    const scalarField3& boundarySdf )
{
}

void advectionSolver3::advect(
    const faceCenteredGrid3& input,
    const vectorField3& flow,
    FloatType dt, faceCenteredGrid3* output,
    const scalarField3& boundarySdf )
{
}