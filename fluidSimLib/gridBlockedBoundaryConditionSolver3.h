//---gridBlockedBoundaryConditionSolver3.h
#ifndef gridBlockedBoundaryConditionSolver3_H
#define gridBlockedBoundaryConditionSolver3_H

#include "gridFractionalBoundaryConditionSolver3.h"
#include "markers3.h"

class gridBlockedBoundaryConditionSolver3 : public gridFractionalBoundaryConditionSolver3
{
public:

    gridBlockedBoundaryConditionSolver3();

    void constrainVelocity(
        faceCenteredGrid3* velocity,
        unsigned int extrapolationDepth = 5) override;

    const markers3& marker() const;

protected:
   
    void onColliderUpdated(
        const size3& gridSize,
        const vector3& gridSpacing,
        const vector3& gridOrigin) override;

private:
    markers3 mMarker;
};

typedef std::shared_ptr< gridBlockedBoundaryConditionSolver3 > gridBlockedBoundaryConditionSolver3Ptr;

#endif


