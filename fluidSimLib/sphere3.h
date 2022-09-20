//---sphere3.h
#ifndef sphere3_H
#define sphere3_H

#include "surface3.h"
#include <memory>

class sphere3 : public surface3
{
public:
    vector3 center;

    //! Radius of the sphere.
    double radius = 1.0;

    //! Constructs a sphere with center at (0, 0, 0) and radius of 1.
    sphere3(
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Constructs a sphere with \p center and \p radius.
    sphere3(
        const vector3& center,
        double radius,
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Copy constructor.
    sphere3(const sphere3& other);

private:
    vector3 closestPointLocal(const vector3& otherPoint) const override;

    double closestDistanceLocal(const vector3& otherPoint) const override;

    bool intersectsLocal(const ray3& ray) const override;

    boundingBox3 boundingBoxLocal() const override;

    vector3 closestNormalLocal(const vector3& otherPoint) const override;

    surfaceRayIntersection3 closestIntersectionLocal(
        const ray3& ray) const override;
};

typedef std::shared_ptr<sphere3> sphere3Ptr;

#endif

