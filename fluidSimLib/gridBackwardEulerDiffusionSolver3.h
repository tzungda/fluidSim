//---gridBackwardEulerDiffusionSolver3.h
#ifndef gridBackwardEulerDiffusionSolver3_H
#define gridBackwardEulerDiffusionSolver3_H

#include "gridDiffusionSolver3.h"
#include "markers3.h"
#include "dataBuffer3.h"
#include "fdmLinearSystemSolver3.h"


class gridBackwardEulerDiffusionSolver3: public gridDiffusionSolver3
{
public:
    enum boundaryType {
        Dirichlet,
        Neumann
    };

   
    gridBackwardEulerDiffusionSolver3(
        boundaryType bndType = Neumann);

   
    void solve(
        const scalarGrid3& source,
        FloatType diffusionCoefficient,
        FloatType timeIntervalInSeconds,
        scalarGrid3* dest,
        const scalarField3& boundarySdf
        = constantScalarField3(mathUtil::maxFloat()),
        const scalarField3& fluidSdf
        = constantScalarField3(-mathUtil::maxFloat())) override;

   
    void solve(
        const extendVectorGrid3& source,
        FloatType diffusionCoefficient,
        FloatType timeIntervalInSeconds,
        extendVectorGrid3* dest,
        const scalarField3& boundarySdf
        = constantScalarField3(mathUtil::maxFloat()),
        const scalarField3& fluidSdf
        = constantScalarField3(-mathUtil::maxFloat())) override;

   
    void solve(
        const faceCenteredGrid3& source,
        FloatType diffusionCoefficient,
        FloatType timeIntervalInSeconds,
        faceCenteredGrid3* dest,
        const scalarField3& boundarySdf
        = constantScalarField3(mathUtil::maxFloat()),
        const scalarField3& fluidSdf
        = constantScalarField3(-mathUtil::maxFloat())) override;

   
    void setLinearSystemSolver(const fdmLinearSystemSolver3Ptr& solver);

private:
    boundaryType mBoundaryType;
    fdmLinearSystem3 mSystem;
    fdmLinearSystemSolver3Ptr mSystemSolver;
    //Array3<char> _markers;
    markers3 mMarkers;

    void buildMarkers(
        const size3& size,
        const std::function<vector3(size_t, size_t, size_t)>& pos,
        const scalarField3& boundarySdf,
        const scalarField3& fluidSdf);

    void buildMatrix(
        const size3& size,
        const vector3& c);

    void buildVectors(
        const dataBuffer3& f,
        const vector3& c);

    void buildVectors(
        const vecDataBuffer3& f,
        const vector3& c,
        size_t component);
};

#endif


