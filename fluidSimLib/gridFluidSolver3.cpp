//---gridFluidSolver3.cpp

#include "gridFluidSolver3.h"
#include "semiLagrangian3.h"
#include "mathUtil.h"
#include "markers3.h"
#include "cubicSemiLagrangian3.h"
#include "gridBackwardEulerDiffusionSolver3.h"
#include "gridFractionalSinglePhasePressureSolver3.h"
#include "surfaceToImplicit3.h"
#include "timer.h"

gridFluidSolver3::gridFluidSolver3() :gridFluidSolver3({ 1, 1, 1 }, { 1, 1, 1 }, { 0, 0, 0 })
{
    mClosedDomainBoundaryFlag = kDirectionAll;
}

gridFluidSolver3::gridFluidSolver3(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& gridOrigin)
{
    mGrids = std::make_shared<gridSystemData3>();
    mGrids->resize(resolution, gridSpacing, gridOrigin);

    setAdvectionSolver(std::make_shared<cubicSemiLagrangian3>());
    //setAdvectionSolver(std::make_shared<semiLagrangian3>());
    setDiffusionSolver(std::make_shared<gridBackwardEulerDiffusionSolver3>());
    setPressureSolver(
        std::make_shared<gridFractionalSinglePhasePressureSolver3>());
    setIsUsingFixedSubTimeSteps(false);

    mClosedDomainBoundaryFlag = kDirectionAll;
}

gridFluidSolver3::~gridFluidSolver3()
{
}

const vector3& gridFluidSolver3::gravity() const
{
    return mGravity;
}

const faceCenteredGrid3Ptr& gridFluidSolver3::velocity() const
{
    return mGrids->velocity();
}

const advectionSolver3Ptr& gridFluidSolver3::advectionSolver() const
{
    return mAdvectionSolver;
}

void gridFluidSolver3::setAdvectionSolver(const advectionSolver3Ptr& newAdvectSolver)
{
    mAdvectionSolver = newAdvectSolver;
}

const gridDiffusionSolver3Ptr& gridFluidSolver3::diffusionSolver() const
{
    return mDiffusionSolver;
}

void gridFluidSolver3::setDiffusionSolver(const gridDiffusionSolver3Ptr& newDiffusionSolver)
{
    mDiffusionSolver = newDiffusionSolver;
}

const gridPressureSolver3Ptr& gridFluidSolver3::pressureSolver() const
{
    return mPressureSolver;
}


void gridFluidSolver3::setPressureSolver(const gridPressureSolver3Ptr& newPressureSolver)
{
    mPressureSolver = newPressureSolver;
    if (mPressureSolver != nullptr)
    {
        mBoundaryConditionSolver
            = mPressureSolver->suggestedBoundaryConditionSolver();

        // set domain boundary flag
        mBoundaryConditionSolver->setClosedDomainBoundaryFlag(
            mClosedDomainBoundaryFlag);
    }
}

void gridFluidSolver3::setEmitter(const gridEmitter3Ptr& newEmitter)
{
    mEmitter = newEmitter;
}

int gridFluidSolver3::closedDomainBoundaryFlag() const
{
    return this->mClosedDomainBoundaryFlag;
}

double gridFluidSolver3::maxCfl() const
{
    return this->mMaxCfl;
}

collider3Ptr gridFluidSolver3::collider() const
{
    return this->mCollider;
}

const cellCenteredScalarGrid3& gridFluidSolver3::colliderSdf() const
{
    return mColliderSdf;
}

void gridFluidSolver3::computeGravity(double timeIntervalInSeconds)
{
    if (mGravity.lengthSquared() > mathUtil::eps())
    {
        faceCenteredGrid3Ptr vel = mGrids->velocity();

        if (std::abs(mGravity.x) > mathUtil::eps())
        {
            vel->forEachUIndex([&](size_t i, size_t j, size_t k) {
                mGrids->velocity()->u(i, j, k) += timeIntervalInSeconds * mGravity.x;
            });
        }

        if (std::abs(mGravity.y) > mathUtil::eps())
        {
            vel->forEachVIndex([&](size_t i, size_t j, size_t k) {
                mGrids->velocity()->v(i, j, k) += timeIntervalInSeconds * mGravity.y;
            });
        }

        if (std::abs(mGravity.z) > mathUtil::eps())
        {
            vel->forEachWIndex([&](size_t i, size_t j, size_t k) {
                mGrids->velocity()->w(i, j, k) += timeIntervalInSeconds * mGravity.z;
            });
        }

        applyBoundaryCondition();
    }
}

void gridFluidSolver3::resizeGrid(
    const size3& newSize,
    const vector3& newGridSpacing,
    const vector3& newGridOrigin)
{
    mGrids->resize(newSize, newGridSpacing, newGridOrigin);
}

const gridSystemData3Ptr& gridFluidSolver3::gridSystemData() const
{
    return mGrids;
}

void gridFluidSolver3::onInitialize()
{
    updateCollider(0.0);

    updateEmitter(0.0);
}

void gridFluidSolver3::onAdvanceTimeStep(double timeIntervalInSeconds)
{
    beginAdvanceTimeStep(timeIntervalInSeconds);

    computeExternalForces(timeIntervalInSeconds);
    computeViscosity(timeIntervalInSeconds);
    computePressure(timeIntervalInSeconds);
    computeAdvection(timeIntervalInSeconds);

    endAdvanceTimeStep(timeIntervalInSeconds);
}

void gridFluidSolver3::onBeginAdvanceTimeStep(double timeIntervalInSeconds)
{
}

void gridFluidSolver3::onEndAdvanceTimeStep(double timeIntervalInSeconds)
{
}

scalarField3Ptr gridFluidSolver3::fluidSdf() const
{
    return std::make_shared<constantScalarField3>(-mathUtil::maxFloat());
}

void gridFluidSolver3::computeExternalForces(double timeIntervalInSeconds)
{
    computeGravity(timeIntervalInSeconds);
}

void gridFluidSolver3::computeViscosity(double timeIntervalInSeconds)
{
    if (mDiffusionSolver != nullptr && mViscosityCoefficient > mathUtil::maxFloat()) {
        auto vel = velocity();
        auto vel0 = std::dynamic_pointer_cast<faceCenteredGrid3>(vel->clone());

        mDiffusionSolver->solve(
            *vel0,
            mViscosityCoefficient,
            timeIntervalInSeconds,
            vel.get(),
            mColliderSdf,
            *fluidSdf());
        applyBoundaryCondition();
    }
}

void gridFluidSolver3::computePressure(double timeIntervalInSeconds)
{
    if (mPressureSolver != nullptr) {
        auto vel = velocity();
        auto vel0 = std::dynamic_pointer_cast<faceCenteredGrid3>(vel->clone());

        mPressureSolver->solve(
            *vel0,
            timeIntervalInSeconds,
            vel.get(),
            mColliderSdf,
            mColliderVel,
            *fluidSdf());
        applyBoundaryCondition();
    }
}

void gridFluidSolver3::computeAdvection(double timeIntervalInSeconds)
{
    faceCenteredGrid3Ptr vel = velocity();
    if (mAdvectionSolver != nullptr) {
        // Solve advections for custom scalar fields
        size_t n = mGrids->numberOfAdvectableScalarData();
        for (size_t i = 0; i < n; ++i) {
            scalarGrid3Ptr grid = mGrids->advectableScalarDataAt(i);
            scalarGrid3Ptr grid0 = grid->clone();
            mAdvectionSolver->advect(
                *grid0,
                *vel,
                timeIntervalInSeconds,
                grid.get(),
                mColliderSdf
            );
            extrapolateIntoCollider(grid.get());
        }

        // Solve advections for custom vector fields
        n = mGrids->numberOfAdvectableVectorData();
        size_t velIdx = mGrids->velocityIndex();
        for (size_t i = 0; i < n; ++i) {
            // Handle velocity layer separately.
            if (i == velIdx) {
                continue;
            }

            const vectorGrid3Ptr grid = mGrids->advectableVectorDataAt(i);
            vectorGrid3Ptr grid0 = grid->clone();

            extendVectorGrid3Ptr extendVecGrid
                = std::dynamic_pointer_cast<extendVectorGrid3>(grid);
            extendVectorGrid3Ptr extendVecGrid0
                = std::dynamic_pointer_cast<extendVectorGrid3>(grid0);
            if (extendVecGrid != nullptr) {
                mAdvectionSolver->advect(
                    *extendVecGrid0,
                    *vel,
                    timeIntervalInSeconds,
                    extendVecGrid.get(),
                    mColliderSdf);
                extrapolateIntoCollider(extendVecGrid.get());
                continue;
            }

            auto faceCentered
                = std::dynamic_pointer_cast<faceCenteredGrid3>(grid);
            auto faceCentered0
                = std::dynamic_pointer_cast<faceCenteredGrid3>(grid0);
            if (faceCentered != nullptr && faceCentered0 != nullptr) {
                mAdvectionSolver->advect(
                    *faceCentered0,
                    *vel,
                    timeIntervalInSeconds,
                    faceCentered.get(),
                    mColliderSdf);
                extrapolateIntoCollider(faceCentered.get());
                continue;
            }
        }

        // Solve velocity advection
        auto vel0 = std::dynamic_pointer_cast<faceCenteredGrid3>(vel->clone());
        mAdvectionSolver->advect(
            *vel0,
            *vel0,
            timeIntervalInSeconds,
            vel.get(),
            mColliderSdf);
        applyBoundaryCondition();
    }
}

void gridFluidSolver3::extrapolateIntoCollider(scalarGrid3* grid)
{
    markers3 marker(grid->dataSize());
    auto pos = grid->dataPosition();
    marker.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (mathUtil::isInsideSdf(mColliderSdf.sample(pos(i, j, k)))) {
            marker(i, j, k) = 0;
        }
        else {
            marker(i, j, k) = 1;
        }
    });

    unsigned int depth = static_cast<unsigned int>(std::ceil(mMaxCfl));
    mathUtil::extrapolateToRegion(
        grid->data(), marker, depth, grid->data());
}

void gridFluidSolver3::extrapolateIntoCollider(extendVectorGrid3* grid)
{
    markers3 marker(grid->dataSize());
    auto pos = grid->dataPosition();
    marker.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (mathUtil::isInsideSdf(mColliderSdf.sample(pos(i, j, k)))) {
            marker(i, j, k) = 0;
        }
        else {
            marker(i, j, k) = 1;
        }
    });

    unsigned int depth = static_cast<unsigned int>(std::ceil(mMaxCfl));
    mathUtil::extrapolateToRegion(
        grid->data(), marker, depth, grid->data());
}

void gridFluidSolver3::extrapolateIntoCollider(faceCenteredGrid3* grid)
{
    auto uPos = grid->uPosition();
    auto vPos = grid->vPosition();
    auto wPos = grid->wPosition();

    markers3 uMarker(grid->uData().size());
    markers3 vMarker(grid->vData().size());
    markers3 wMarker(grid->wData().size());

    uMarker.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (mathUtil::isInsideSdf(mColliderSdf.sample(uPos(i, j, k)))) {
            uMarker(i, j, k) = 0;
        }
        else {
            uMarker(i, j, k) = 1;
        }
    });

    vMarker.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (mathUtil::isInsideSdf(mColliderSdf.sample(vPos(i, j, k)))) {
            vMarker(i, j, k) = 0;
        }
        else {
            vMarker(i, j, k) = 1;
        }
    });

    wMarker.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (mathUtil::isInsideSdf(mColliderSdf.sample(wPos(i, j, k)))) {
            wMarker(i, j, k) = 0;
        }
        else {
            wMarker(i, j, k) = 1;
        }
    });

    unsigned int depth = static_cast<unsigned int>(std::ceil(mMaxCfl));
    mathUtil::extrapolateToRegion(grid->uData(), uMarker, depth, grid->uData());
    mathUtil::extrapolateToRegion(grid->vData(), vMarker, depth, grid->vData());
    mathUtil::extrapolateToRegion(grid->wData(), wMarker, depth, grid->wData());

}

void gridFluidSolver3::applyBoundaryCondition()
{
    faceCenteredGrid3Ptr vel = mGrids->velocity();

    if (vel != nullptr && mBoundaryConditionSolver != nullptr)
    {
        unsigned int depth = static_cast<unsigned int>(std::ceil(mMaxCfl));
        mBoundaryConditionSolver->constrainVelocity(vel.get(), depth);
    }
}

void gridFluidSolver3::beginAdvanceTimeStep(double timeIntervalInSeconds)
{
    size3 res = mGrids->resolution();
    vector3 h = mGrids->gridSpacing();
    vector3 o = mGrids->origin();

    // reserve memory
    mColliderSdf.resize(res, h, o);
    mColliderVel.resize(res, h, o);

    // update collider and emitter
    updateCollider(timeIntervalInSeconds);
    updateEmitter(timeIntervalInSeconds);

    // rasterize collider into SDF
    if (mCollider != nullptr)
    {
        auto pos = mColliderSdf.dataPosition();
        surface3Ptr surface = mCollider->surface();
        implicitSurface3Ptr implicitSurface
            = std::dynamic_pointer_cast<implicitSurface3>(surface);
        if (implicitSurface == nullptr) {
            implicitSurface = std::make_shared<surfaceToImplicit3>(surface);
        }

        mColliderSdf.fill([&](const vector3& pt) {
            return implicitSurface->signedDistance(pt);
        });

        mColliderVel.fill([&](const vector3& pt) {
            return mCollider->velocityAt(pt);
        });
    }
    else
    {
        mColliderSdf.fill(mathUtil::maxFloat());
        mColliderVel.fill({ 0, 0, 0 });
    }

    // update boundary condition solver
    if (mBoundaryConditionSolver != nullptr)
    {
        mBoundaryConditionSolver->updateCollider(
            mCollider,
            mGrids->resolution(),
            mGrids->gridSpacing(),
            mGrids->origin());
    }

    // Apply boundary condition to the velocity field in case the field got
    // updated externally.
    applyBoundaryCondition();

    // Invoke callback
    onBeginAdvanceTimeStep(timeIntervalInSeconds);
}

void gridFluidSolver3::endAdvanceTimeStep(double timeIntervalInSeconds)
{
    onEndAdvanceTimeStep(timeIntervalInSeconds);
}

void gridFluidSolver3::updateCollider(double timeIntervalInSeconds)
{
    if (mCollider != nullptr)
    {
        mCollider->update(currentTimeInSeconds(), timeIntervalInSeconds);
    }
}

void gridFluidSolver3::updateEmitter(double timeIntervalInSeconds)
{
    if (mEmitter != nullptr)
    {
        mEmitter->update(currentTimeInSeconds(), timeIntervalInSeconds);
    }
}

