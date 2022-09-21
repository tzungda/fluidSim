//---semiLagrangian3.cpp

//#include <cmath>
#include "semiLagrangian3.h"
#include "mathUtil.h"

semiLagrangian3::semiLagrangian3()
{
}

semiLagrangian3::~semiLagrangian3()
{
}

void semiLagrangian3::advect(
    const scalarGrid3& input,
    const vectorField3& flow,
    FloatType dt,
    scalarGrid3* output,
    const scalarField3& boundarySdf )
{
    scalarGrid3::DataPositionFunc outputDataPos = output->dataPosition();
    auto inputSamplerFunc = getScalarSamplerFunc(input);
    scalarGrid3::DataPositionFunc inputDataPos = input.dataPosition();

    FloatType h = std::min(std::min(output->gridSpacing().x, output->gridSpacing().y), output->gridSpacing().z);

    output->forEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        if (boundarySdf.sample(inputDataPos(i, j, k)) > 0.0) {
            vector3 pt = backTrace(
                flow, dt, h, outputDataPos(i, j, k), boundarySdf);
            (*output)( i, j, k ) = inputSamplerFunc(pt);
        }
    });
}

void semiLagrangian3::advect(
    const extendVectorGrid3& input,
    const vectorField3& flow,
    FloatType dt,
    extendVectorGrid3* output,
    const scalarField3& boundarySdf )
{
    auto inputSamplerFunc = getVectorSamplerFunc(input);

    FloatType h = std::min(std::min(output->gridSpacing().x, output->gridSpacing().y), output->gridSpacing().z);

    vectorGrid3::DataPositionFunc outputDataPos = output->dataPosition();
    vectorGrid3::DataPositionFunc inputDataPos = input.dataPosition();

    output->forEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        if (boundarySdf.sample(inputDataPos(i, j, k)) > 0.0) {
            vector3 pt = backTrace(
                flow, dt, h, outputDataPos(i, j, k), boundarySdf);
            (*output)( i, j, k ) = inputSamplerFunc(pt);
        }
    });
}

void semiLagrangian3::advect( const faceCenteredGrid3& input, const vectorField3& flow,
    FloatType dt, faceCenteredGrid3* output, const scalarField3& boundarySdf )
{
    auto inputSamplerFunc = getVectorSamplerFunc(input);
    FloatType h = std::min(std::min(output->gridSpacing().x, output->gridSpacing().y), output->gridSpacing().z);

    output->forEachUIndex([&](size_t i, size_t j, size_t k) {
        if (boundarySdf.sample(input.uPosition(i, j, k)) > 0.0) {
            vector3 pt = backTrace(
                flow, dt, h, output->uPosition(i, j, k), boundarySdf);
            output->u(i, j, k) = inputSamplerFunc(pt).x;
        }
    });

    output->forEachVIndex([&](size_t i, size_t j, size_t k) {
        if (boundarySdf.sample(input.vPosition(i, j, k)) > 0.0) {
            vector3 pt = backTrace(
                flow, dt, h, output->vPosition(i, j, k), boundarySdf);
            output->v(i, j, k) = inputSamplerFunc(pt).y;
        }
    });

    output->forEachWIndex([&](size_t i, size_t j, size_t k) {
        if (boundarySdf.sample(input.wPosition(i, j, k)) > 0.0) {
            vector3 pt = backTrace(
                flow, dt, h, output->wPosition(i, j, k), boundarySdf);
            output->w(i, j, k) = inputSamplerFunc(pt).z;
        }
    });
}

std::function<FloatType(const vector3&)>
semiLagrangian3::getScalarSamplerFunc(const scalarGrid3& input) const
{
    return input.sampler();
}


std::function<vector3(const vector3&)>
semiLagrangian3::getVectorSamplerFunc(const extendVectorGrid3& input) const
{
    return input.sampler();
}


std::function<vector3(const vector3&)>
semiLagrangian3::getVectorSamplerFunc(const faceCenteredGrid3& input) const
{
    return input.sampler();
}

vector3 semiLagrangian3::backTrace(
    const vectorField3& flow,
    FloatType dt,
    FloatType h,
    const vector3& startPt,
    const scalarField3& boundarySdf)
{
    FloatType remainingT = dt;
    vector3 pt0 = startPt;
    vector3 pt1 = startPt;

    while ( remainingT > mathUtil::eps() )
    {
        // adaptive time-stepping
        vector3 vel0 = flow.sample(pt0);
        FloatType numSubSteps
            = std::max<FloatType>(std::ceil(vel0.length() * remainingT / h), (FloatType)1.0);
        dt = remainingT / numSubSteps;

        // mid-point rule
        vector3 midPt = pt0 - (FloatType)0.5 * dt * vel0;
        vector3 midVel = flow.sample(midPt);
        pt1 = pt0 - dt * midVel;

        // boundary handling
        FloatType phi0 = boundarySdf.sample(pt0);
        FloatType phi1 = boundarySdf.sample(pt1);

        if (phi0 * phi1 < 0.0)
        {
            FloatType w = std::fabs(phi1) / (std::fabs(phi0) + std::fabs(phi1));
            pt1 = w * pt0 + ((FloatType)1.0 - w) * pt1;
            break;
        }

        remainingT -= dt;
        pt0 = pt1;
    }

    return pt1;
}