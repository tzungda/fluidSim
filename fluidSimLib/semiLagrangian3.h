//---semiLagrangian3.h
#ifndef semiLagrangian3_H
#define semiLagrangian3_H

#include "advectionSolver3.h"

class semiLagrangian3 : public advectionSolver3
{
public:
semiLagrangian3();
virtual ~semiLagrangian3();

    virtual void advect(
        const scalarGrid3& input,
        const vectorField3& flow,
        double dt,
        scalarGrid3* output,
        const scalarField3& boundarySdf = constantScalarField3(std::numeric_limits<double>::max()) ) override;

    virtual void advect(
        const extendVectorGrid3& input,
        const vectorField3& flow,
        double dt,
        extendVectorGrid3* output,
        const scalarField3& boundarySdf = constantScalarField3(std::numeric_limits<double>::max()) ) override;

    virtual void advect(
        const faceCenteredGrid3& input,
        const vectorField3& flow,
        double dt, faceCenteredGrid3* output,
        const scalarField3& boundarySdf = constantScalarField3(std::numeric_limits<double>::max()) ) override;

protected:

    virtual std::function<double(const vector3&)>
        getScalarSamplerFunc(const scalarGrid3& input) const;


    virtual std::function<vector3(const vector3&)>
        getVectorSamplerFunc(const extendVectorGrid3& input) const;


    virtual std::function<vector3(const vector3&)>
        getVectorSamplerFunc(const faceCenteredGrid3& input) const;

private:
    vector3 backTrace(
        const vectorField3& flow,
        double dt,
        double h,
        const vector3& pt0,
        const scalarField3& boundarySdf);

};

#endif

