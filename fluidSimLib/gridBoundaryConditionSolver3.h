//---gridBoundaryConditionSolver3.h
#ifndef gridBoundaryConditionSolver3_H
#define gridBoundaryConditionSolver3_H

#include "collider3.h"
#include "faceCenteredGrid3.h"

class gridBoundaryConditionSolver3
{
public:
   
    gridBoundaryConditionSolver3();

   
    virtual ~gridBoundaryConditionSolver3();

   
    const collider3Ptr& collider() const;

   
    void updateCollider(
        const collider3Ptr& newCollider,
        const size3& gridSize,
        const vector3& gridSpacing,
        const vector3& gridOrigin);

   
    int closedDomainBoundaryFlag() const;

    //! Sets the closed domain boundary flag.
    void setClosedDomainBoundaryFlag(int flag);

    //!
    //! Constrains the velocity field to conform the collider boundary.
    //!
    //! \param velocity Input and output velocity grid.
    //! \param extrapolationDepth Number of inner-collider grid cells that
    //!     velocity will get extrapolated.
    //!
    virtual void constrainVelocity(
        faceCenteredGrid3* velocity,
        unsigned int extrapolationDepth = 5) = 0;

protected:
    //! Invoked when a new collider is set.
    virtual void onColliderUpdated(
        const size3& gridSize,
        const vector3& gridSpacing,
        const vector3& gridOrigin) = 0;

    //! Returns the size of the velocity grid to be constrained.
    const size3& gridSize() const;

    //! Returns the spacing of the velocity grid to be constrained.
    const vector3& gridSpacing() const;

    //! Returns the origin of the velocity grid to be constrained.
    const vector3& gridOrigin() const;

private:
    collider3Ptr mCollider;
    size3 mGridSize;
    vector3 mGridSpacing;
    vector3 mGridOrigin;
    int mClosedDomainBoundaryFlag;
};

typedef std::shared_ptr< gridBoundaryConditionSolver3 > gridBoundaryConditionSolver3Ptr;

#endif

