//---plane3.h
#ifndef plane3_H
#define plane3_H

#include "surface3.h"

class plane3: public surface3
{
public:

    //! Plane normal.
    vector3 normal = vector3(0, 1, 0);

    //! Point that lies on the plane.
    vector3 point;

    //! Constructs a plane that crosses (0, 0, 0) with surface normal (0, 1, 0).
    plane3(
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Constructs a plane that cross \p point with surface normal \p normal.
    plane3(
        const vector3& normal,
        const vector3& point,
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Constructs a plane with three points on the surface. The normal will be
    //! set using the counter clockwise direction.
    plane3(
        const vector3& point0,
        const vector3& point1,
        const vector3& point2,
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Copy constructor.
    plane3(const plane3& other);



protected:
    vector3 closestPointLocal(const vector3& otherPoint) const override;

    bool intersectsLocal(const ray3& ray) const override;

    boundingBox3 boundingBoxLocal() const override;

    vector3 closestNormalLocal(const vector3& otherPoint) const override;

    surfaceRayIntersection3 closestIntersectionLocal(
        const ray3& ray) const override;
};

typedef std::shared_ptr<plane3> plane3Ptr;

#endif


