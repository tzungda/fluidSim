//---gridBoundaryConditionSolver3.cpp

#include "gridBoundaryConditionSolver3.h"
#include "boundaryDef.h"

gridBoundaryConditionSolver3::gridBoundaryConditionSolver3()
{
    mClosedDomainBoundaryFlag = kDirectionAll;
}

gridBoundaryConditionSolver3::~gridBoundaryConditionSolver3()
{
}

const collider3Ptr& gridBoundaryConditionSolver3::collider() const
{
    return mCollider;
}


void gridBoundaryConditionSolver3::updateCollider(
    const collider3Ptr& newCollider,
    const size3& gridSize,
    const vector3& gridSpacing,
    const vector3& gridOrigin)
{
    mCollider = newCollider;
    mGridSize = gridSize;
    mGridSpacing = gridSpacing;
    mGridOrigin = gridOrigin;

    onColliderUpdated(gridSize, gridSpacing, gridOrigin);
}

int gridBoundaryConditionSolver3::closedDomainBoundaryFlag() const
{
    return mClosedDomainBoundaryFlag;
}

void gridBoundaryConditionSolver3::setClosedDomainBoundaryFlag(int flag)
{
    mClosedDomainBoundaryFlag = flag;
}

const size3& gridBoundaryConditionSolver3::gridSize() const
{
    return mGridSize;
}

const vector3& gridBoundaryConditionSolver3::gridSpacing() const
{
    return mGridSpacing;
}

const vector3& gridBoundaryConditionSolver3::gridOrigin() const
{
    return mGridOrigin;
}

