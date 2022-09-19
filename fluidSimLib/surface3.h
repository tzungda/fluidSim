//---surface3.h
#ifndef surface3_H
#define surface3_H

#include <memory>

#include "transform3.h"
#include "boundingBox3.h"

//==========================================================
struct surfaceRayIntersection3 {
    bool isIntersecting = false;
    FloatType t = std::numeric_limits<FloatType>::max();;
    vector3 point;
    vector3 normal;
};

//==========================================================
class surface3
{
public:
    surface3(
        const transform3& _transform = transform3(),
        bool _isNormalFlipped = false);

    surface3(const surface3& other);

    virtual ~surface3();

    vector3 closestPoint(const vector3& otherPoint) const;

    boundingBox3 boundingBox() const;

    bool intersects(const ray3& ray) const;

    FloatType closestDistance(const vector3& otherPoint) const;

    surfaceRayIntersection3 closestIntersection(const ray3& ray) const;

    vector3 closestNormal(const vector3& otherPoint) const;

public:
    transform3 transform;
    bool isNormalFlipped = false;

protected:
    //! Returns the closest point from the given point \p otherPoint to the
    //! surface in local frame.
    virtual vector3 closestPointLocal(const vector3& otherPoint) const = 0;

    //! Returns the bounding box of this surface object in local frame.
    virtual boundingBox3 boundingBoxLocal() const = 0;

    //! Returns the closest intersection point for given \p ray in local frame.
    virtual surfaceRayIntersection3 closestIntersectionLocal(
        const ray3& ray) const = 0;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint in local frame.
    virtual vector3 closestNormalLocal(const vector3& otherPoint) const = 0;

    //! Returns true if the given \p ray intersects with this surface object
    //! in local frame.
    virtual bool intersectsLocal(const ray3& ray) const;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface in local frame.
    virtual FloatType closestDistanceLocal(const vector3& otherPoint) const;
};

typedef std::shared_ptr<surface3> surface3Ptr;

#endif

