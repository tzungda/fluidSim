//---gridBlockedBoundaryConditionSolver3.cpp

#include "gridBlockedBoundaryConditionSolver3.h"
#include "mathUtil.h"

static const char kFluid = 1;
static const char kCollider = 0;

gridBlockedBoundaryConditionSolver3::gridBlockedBoundaryConditionSolver3() 
{
}

void gridBlockedBoundaryConditionSolver3::constrainVelocity(
    faceCenteredGrid3* velocity,
    unsigned int extrapolationDepth)
{
    gridFractionalBoundaryConditionSolver3::constrainVelocity(
        velocity, extrapolationDepth);

    // No-flux: project the velocity at the marker interface
    size3 size = velocity->resolution();

    auto uPos = velocity->uPosition();
    auto vPos = velocity->vPosition();
    auto wPos = velocity->wPosition();

    mMarker.forEachIndex([&](size_t i, size_t j, size_t k) 
    {
        if (mMarker(i, j, k) == kCollider) 
        {
            if (i > 0 && mMarker(i - 1, j, k) == kFluid) 
            {
                vector3 colliderVel = collider()->velocityAt(uPos(i, j, k));
                velocity->u( i, j, k ) = colliderVel.x;
            }
            if (i < size.x - 1 && mMarker(i + 1, j, k) == kFluid) 
            {
                vector3 colliderVel
                    = collider()->velocityAt(uPos(i + 1, j, k));
                velocity->u( i + 1, j, k ) = colliderVel.x;
            }
            if (j > 0 && mMarker(i, j - 1, k) == kFluid) 
            {
                vector3 colliderVel = collider()->velocityAt(vPos(i, j, k));
                velocity->v( i, j, k ) = colliderVel.y;
            }
            if (j < size.y - 1 && mMarker(i, j + 1, k) == kFluid) 
            {
                vector3 colliderVel
                    = collider()->velocityAt(vPos(i, j + 1, k));
                velocity->v( i, j + 1, k ) = colliderVel.y;
            }
            if (k > 0 && mMarker(i, j, k - 1) == kFluid) 
            {
                vector3 colliderVel = collider()->velocityAt(wPos(i, j, k));
                velocity->w( i, j, k ) = colliderVel.z;
            }
            if (k < size.z - 1 && mMarker(i, j, k + 1) == kFluid) 
            {
                vector3 colliderVel
                    = collider()->velocityAt(wPos(i, j, k + 1));
                velocity->w( i, j, k + 1 ) = colliderVel.z;
            }
        }
        });
}

const markers3& gridBlockedBoundaryConditionSolver3::marker() const
{
    return mMarker;
}

void gridBlockedBoundaryConditionSolver3::onColliderUpdated(
    const size3& gridSize,
    const vector3& gridSpacing,
    const vector3& gridOrigin)
{
    gridFractionalBoundaryConditionSolver3::onColliderUpdated(
        gridSize, gridSpacing, gridOrigin);

    const cellCenteredScalarGrid3& sdf = colliderSdf();

    mMarker.resize(gridSize);
    mMarker.forEachIndex([&](size_t i, size_t j, size_t k) {
        if ( mathUtil::isInsideSdf(sdf(i, j, k))) {
            mMarker(i, j, k) = kCollider;
        } else {
            mMarker(i, j, k) = kFluid;
        }
        });
}


