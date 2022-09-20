//---gridFractionalBoundaryConditionSolver3.h
#ifndef gridFractionalBoundaryConditionSolver3_H
#define gridFractionalBoundaryConditionSolver3_H

#include "cellCenteredScalarGrid3.h"
#include "gridBoundaryConditionSolver3.h"

class gridFractionalBoundaryConditionSolver3: public gridBoundaryConditionSolver3
{
public:
    //! Default constructor.
    gridFractionalBoundaryConditionSolver3();

    //! Default destructor.
    virtual ~gridFractionalBoundaryConditionSolver3();

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    void constrainVelocity(
        faceCenteredGrid3* velocity,
        unsigned int extrapolationDepth = 5) override;

    //! Returns the signed distance field of the collider.
    const cellCenteredScalarGrid3& colliderSdf() const;

protected:
    //! Invoked when a new collider is set.
    void onColliderUpdated(
        const size3& gridSize,
        const vector3& gridSpacing,
        const vector3& gridOrigin) override;

private:
    cellCenteredScalarGrid3 mColliderSdf;
};

typedef std::shared_ptr<gridFractionalBoundaryConditionSolver3> gridFractionalBoundaryConditionSolver3Ptr;

#endif

