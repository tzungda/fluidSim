//---surfaceToImplicit3.h
#ifndef surfaceToImplicit3_H
#define surfaceToImplicit3_H

#include "implicitSurface3.h"

class surfaceToImplicit3: public implicitSurface3
{
public:

    //! Constructs an instance with generic Surface3 instance.
    surfaceToImplicit3(
        const surface3Ptr& surface,
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Copy constructor.
    surfaceToImplicit3(const surfaceToImplicit3& other);

    //! Returns the raw surface instance.
    surface3Ptr surface() const;


protected:
    vector3 closestPointLocal(const vector3& otherPoint) const override;

    double closestDistanceLocal(const vector3& otherPoint) const override;

    bool intersectsLocal(const ray3& ray) const override;

    boundingBox3 boundingBoxLocal() const override;

    vector3 closestNormalLocal(
        const vector3& otherPoint) const override;

    double signedDistanceLocal(const vector3& otherPoint) const override;

    surfaceRayIntersection3 closestIntersectionLocal(
        const ray3& ray) const override;

private:
    surface3Ptr mSurface;
};

typedef std::shared_ptr<surfaceToImplicit3> surfaceToImplicit3Ptr;

#endif

