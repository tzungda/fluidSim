//---gridSmokeSolver3.cpp

#include "gridSmokeSolver3.h"
#include "mathUtil.h"

gridSmokeSolver3::gridSmokeSolver3()
    : gridSmokeSolver3({1, 1, 1}, {1, 1, 1}, {0, 0, 0})
{
}

gridSmokeSolver3::gridSmokeSolver3(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& gridOrigin)
    : gridFluidSolver3(resolution, gridSpacing, gridOrigin)
{
    const gridSystemData3Ptr& grids = gridSystemData();
    mSmokeDensityDataId = grids->addAdvectableScalarData(
        std::make_shared<cellCenteredScalarGrid3>(resolution, gridSpacing, gridOrigin), 0.0);
    mTemperatureDataId = grids->addAdvectableScalarData(
        std::make_shared<cellCenteredScalarGrid3>(resolution, gridSpacing, gridOrigin), 0.0);
}

gridSmokeSolver3::~gridSmokeSolver3()
{
}

double gridSmokeSolver3::smokeDiffusionCoefficient() const
{
    return mSmokeDiffusionCoefficient;
}

void gridSmokeSolver3::setSmokeDiffusionCoefficient(double newValue)
{
    mSmokeDiffusionCoefficient = std::max(newValue, 0.0);
}

double gridSmokeSolver3::temperatureDiffusionCoefficient() const
{
    return mTemperatureDiffusionCoefficient;
}

void gridSmokeSolver3::setTemperatureDiffusionCoefficient(double newValue)
{
    mTemperatureDiffusionCoefficient = std::max(newValue, 0.0);
}

double gridSmokeSolver3::buoyancySmokeDensityFactor() const
{
    return mBuoyancySmokeDensityFactor;
}

void gridSmokeSolver3::setBuoyancySmokeDensityFactor(double newValue)
{
    mBuoyancySmokeDensityFactor = newValue;
}

double gridSmokeSolver3::buoyancyTemperatureFactor() const
{
    return mBuoyancyTemperatureFactor;
}

void gridSmokeSolver3::setBuoyancyTemperatureFactor(double newValue)
{
    mBuoyancyTemperatureFactor = newValue;
}

double gridSmokeSolver3::smokeDecayFactor() const
{
    return mSmokeDecayFactor;
}

void gridSmokeSolver3::setSmokeDecayFactor(double newValue)
{
    mSmokeDecayFactor = mathUtil::clamp(newValue, 0.0, 1.0);
}

double gridSmokeSolver3::smokeTemperatureDecayFactor() const
{
    return mTemperatureDecayFactor;
}

void gridSmokeSolver3::setTemperatureDecayFactor(double newValue)
{
    mTemperatureDecayFactor = mathUtil::clamp(newValue, 0.0, 1.0);
}

scalarGrid3Ptr gridSmokeSolver3::smokeDensity() const
{
    return gridSystemData()->advectableScalarDataAt(mSmokeDensityDataId);
}

scalarGrid3Ptr gridSmokeSolver3::temperature() const
{
    return gridSystemData()->advectableScalarDataAt(mTemperatureDataId);
}

void gridSmokeSolver3::onEndAdvanceTimeStep(double timeIntervalInSeconds)
{
    computeDiffusion(timeIntervalInSeconds);
}

void gridSmokeSolver3::computeExternalForces(double timeIntervalInSeconds)
{
    computeBuoyancyForce(timeIntervalInSeconds);
}

void gridSmokeSolver3::computeDiffusion(double timeIntervalInSeconds)
{
    if (diffusionSolver() != nullptr)
    {
        if (mSmokeDiffusionCoefficient > mathUtil::eps())
        {
            scalarGrid3Ptr den = smokeDensity();
            cellCenteredScalarGrid3Ptr den0 = std::dynamic_pointer_cast<cellCenteredScalarGrid3>( den->clone() );

            diffusionSolver()->solve(
                *den0,
                mSmokeDiffusionCoefficient,
                timeIntervalInSeconds,
                den.get(),
                colliderSdf());
            extrapolateIntoCollider(den.get());
        }

        if (mTemperatureDiffusionCoefficient > mathUtil::eps())
        {
            scalarGrid3Ptr temp = smokeDensity();
            cellCenteredScalarGrid3Ptr temp0 = std::dynamic_pointer_cast<cellCenteredScalarGrid3>( temp->clone() );

            diffusionSolver()->solve(
                *temp0,
                mTemperatureDiffusionCoefficient,
                timeIntervalInSeconds,
                temp.get(),
                colliderSdf());
            extrapolateIntoCollider(temp.get());
        }
    }

    scalarGrid3Ptr den = smokeDensity();
    den->forEachDataPointIndex(
        [&](size_t i, size_t j, size_t k) {
        (*den)(i, j, k) *= 1.0 - mSmokeDecayFactor;
    });
    scalarGrid3Ptr temp = temperature();
    temp->forEachDataPointIndex(
        [&](size_t i, size_t j, size_t k) {
        (*temp)(i, j, k) *= 1.0 - mTemperatureDecayFactor;
    });
}

void gridSmokeSolver3::computeBuoyancyForce(double timeIntervalInSeconds)
{
    auto grids = gridSystemData();
    auto vel = grids->velocity();

    vector3 up(0.0, 1.0, 0.0);
    if (gravity().lengthSquared() > mathUtil::eps())
    {
        up = -gravity().normalized();
    }

    if (std::abs(mBuoyancySmokeDensityFactor) > mathUtil::eps() ||
        std::abs(mBuoyancyTemperatureFactor) > mathUtil::eps())
    {
        scalarGrid3Ptr den = smokeDensity();
        scalarGrid3Ptr temp = temperature();

        double tAmb = 0.0;
        temp->forEachCellIndex([&](size_t i, size_t j, size_t k) {
            tAmb += (*temp)(i, j, k);
        });
        tAmb /= static_cast<double>(
            temp->resolution().x * temp->resolution().y * temp->resolution().z);

        vectorGrid3::DataPositionFunc uPos = vel->uPosition();
        vectorGrid3::DataPositionFunc vPos = vel->vPosition();
        vectorGrid3::DataPositionFunc wPos = vel->wPosition();

        if (std::abs(up.x) > mathUtil::eps())
        {
            vel->forEachUIndex([&](size_t i, size_t j, size_t k) {
                vector3 pt = uPos(i, j, k);
                double fBuoy
                    = mBuoyancySmokeDensityFactor * den->sample(pt)
                    + mBuoyancyTemperatureFactor * (temp->sample(pt) - tAmb);
                vel->u( i, j, k ) += timeIntervalInSeconds * fBuoy * up.x;
            });
        }

        if (std::abs(up.y) > mathUtil::eps())
        {
            vel->forEachVIndex([&](size_t i, size_t j, size_t k) {
                vector3 pt = vPos(i, j, k);
                double fBuoy
                    = mBuoyancySmokeDensityFactor * den->sample(pt)
                    + mBuoyancyTemperatureFactor * (temp->sample(pt) - tAmb);
                vel->v( i, j, k ) += timeIntervalInSeconds * fBuoy * up.y;
            });
        }

        if (std::abs(up.z) > mathUtil::eps())
        {
            vel->forEachWIndex([&](size_t i, size_t j, size_t k) {
                vector3 pt = wPos(i, j, k);
                double fBuoy
                    = mBuoyancySmokeDensityFactor * den->sample(pt)
                    + mBuoyancyTemperatureFactor * (temp->sample(pt) - tAmb);
                vel->w( i, j, k ) += timeIntervalInSeconds * fBuoy * up.z;
            });
        }

        applyBoundaryCondition();
    }
}
