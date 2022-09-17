//--- box3.h
#ifndef box3_H
#define box3_H

#include "surface3.h"

class box3 : public surface3
{
public:
    boundingBox3 bound
        = boundingBox3(vector3(), vector3(1.0, 1.0, 1.0));

    //! Constructs (0, 0, 0) x (1, 1, 1) box.
    box3(
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Constructs a box with given \p lowerCorner and \p upperCorner.
    box3(
        const vector3& lowerCorner,
        const vector3& upperCorner,
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Constructs a box with boundingBox3 instance.
    explicit box3(
        const boundingBox3& boundingBox,
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Copy constructor.
    box3(const box3& other);

protected:
    // Surface3 implementations

    vector3 closestPointLocal(const vector3& otherPoint) const override;

    bool intersectsLocal(const ray3& ray) const override;

    boundingBox3 boundingBoxLocal() const override;

    vector3 closestNormalLocal(const vector3& otherPoint) const override;

    surfaceRayIntersection3 closestIntersectionLocal(
        const ray3& ray) const override;
};

typedef std::shared_ptr<box3> box3Ptr;

#endif

