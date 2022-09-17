
//--- advectionSolver3.h
#ifndef advectionSolver3_H
#define advectionSolver3_H

#include "faceCenteredGrid3.h"
#include "extendVectorGrid3.h"
#include "scalarField3.h"
#include "scalarGrid3.h"
#include "vectorField3.h"

class advectionSolver3
{
public:
advectionSolver3();
virtual ~advectionSolver3();

virtual void advect(
        const scalarGrid3& input,
        const vectorField3& flow,
        double dt,
        scalarGrid3* output,
        const scalarField3& boundarySdf = constantScalarField3(std::numeric_limits<double>::max()) );

virtual void advect(
        const extendVectorGrid3& input,
        const vectorField3& flow,
        double dt,
        extendVectorGrid3* output,
        const scalarField3& boundarySdf = constantScalarField3(std::numeric_limits<double>::max()) );

virtual void advect(
        const faceCenteredGrid3& input,
        const vectorField3& flow,
        double dt, faceCenteredGrid3* output,
        const scalarField3& boundarySdf = constantScalarField3(std::numeric_limits<double>::max()) );
   
};

typedef std::shared_ptr< advectionSolver3 > advectionSolver3Ptr;

#endif