//---gridDiffusionSolver3.h
#ifndef gridDiffusionSolver3_H
#define gridDiffusionSolver3_H

#include "scalarGrid3.h"
#include "extendVectorGrid3.h"
#include "faceCenteredGrid3.h"
#include "mathUtil.h"

class gridDiffusionSolver3
{
public:
    gridDiffusionSolver3();

    virtual ~gridDiffusionSolver3();


    virtual void solve(
        const scalarGrid3& source,
        FloatType diffusionCoefficient,
        FloatType timeIntervalInSeconds,
        scalarGrid3* dest,
        const scalarField3& boundarySdf = constantScalarField3(mathUtil::maxFloat()),
        const scalarField3& fluidSdf = constantScalarField3(-mathUtil::maxFloat())) = 0;


    virtual void solve(
        const extendVectorGrid3& source,
        FloatType diffusionCoefficient,
        FloatType timeIntervalInSeconds,
        extendVectorGrid3* dest,
        const scalarField3& boundarySdf = constantScalarField3(mathUtil::maxFloat()),
        const scalarField3& fluidSdf = constantScalarField3(-mathUtil::maxFloat())) = 0;


    virtual void solve(
        const faceCenteredGrid3& source,
        FloatType diffusionCoefficient,
        FloatType timeIntervalInSeconds,
        faceCenteredGrid3* dest,
        const scalarField3& boundarySdf = constantScalarField3(mathUtil::maxFloat()),
        const scalarField3& fluidSdf = constantScalarField3(-mathUtil::maxFloat())) = 0;
};

typedef std::shared_ptr<gridDiffusionSolver3> gridDiffusionSolver3Ptr;

#endif

