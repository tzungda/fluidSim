//---surfaceToImplicit3.cpp

#include "surfaceToImplicit3.h"


surfaceToImplicit3::surfaceToImplicit3(
    const surface3Ptr& surface,
    const transform3& transform,
    bool isNormalFlipped)
    : implicitSurface3(transform, isNormalFlipped)
    , mSurface(surface) 
{
}

surfaceToImplicit3::surfaceToImplicit3(const surfaceToImplicit3& other) :
    implicitSurface3(other),
    mSurface(other.mSurface) 
{
}

surface3Ptr surfaceToImplicit3::surface() const 
{
    return mSurface;
}

vector3 surfaceToImplicit3::closestPointLocal( const vector3& otherPoint) const 
{
    return mSurface->closestPoint(otherPoint);
}

vector3 surfaceToImplicit3::closestNormalLocal( const vector3& otherPoint) const 
{
    return mSurface->closestNormal(otherPoint);
}

FloatType surfaceToImplicit3::closestDistanceLocal( const vector3& otherPoint) const
{
    return mSurface->closestDistance(otherPoint);
}

bool surfaceToImplicit3::intersectsLocal(const ray3& ray) const 
{
    return mSurface->intersects(ray);
}

surfaceRayIntersection3 surfaceToImplicit3::closestIntersectionLocal( const ray3& ray) const 
{
    return mSurface->closestIntersection(ray);
}

boundingBox3 surfaceToImplicit3::boundingBoxLocal() const 
{
    return mSurface->boundingBox();
}

FloatType surfaceToImplicit3::signedDistanceLocal( const vector3& otherPoint) const 
{
    vector3 x = mSurface->closestPoint(otherPoint);
    vector3 n = mSurface->closestNormal(otherPoint);
    n = (isNormalFlipped) ? -n : n;
    if (n.dot(otherPoint - x) < 0.0)
    {
        return -x.distanceTo(otherPoint);
    } 
    else
    {
        return x.distanceTo(otherPoint);
    }
}

