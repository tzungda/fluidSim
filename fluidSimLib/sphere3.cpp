//---sphere3.cpp

#include <cmath>

#include "sphere3.h"

sphere3::sphere3(
    const transform3& transform,
    bool isNormalFlipped): surface3(transform, isNormalFlipped)
{
}


sphere3::sphere3(
    const vector3& center,
    FloatType radius,
    const transform3& transform,
    bool isNormalFlipped):
    surface3(transform, isNormalFlipped)
    , center(center)
    , radius(radius)
{
}


sphere3::sphere3(const sphere3& other):
    surface3(other),
    center(other.center),
    radius(other.radius)
{
}


vector3 sphere3::closestPointLocal(const vector3& otherPoint) const
{
    return radius * closestNormalLocal(otherPoint) + center;
}

FloatType sphere3::closestDistanceLocal(const vector3& otherPoint) const
{
    return std::fabs(center.distanceTo(otherPoint) - radius);
}

bool sphere3::intersectsLocal(const ray3& ray) const
{
    vector3 r = ray.origin - center;
    FloatType b = ray.direction.dot(r);
    FloatType c = r.lengthSquared() - radius * radius;
    FloatType d = b * b - c;

    if (d > 0.0) 
    {
        d = std::sqrt(d);
        FloatType tMin = -b - d;
        FloatType tMax = -b + d;

        if (tMin < 0.0) 
        {
            tMin = tMax;
            tMax = std::numeric_limits<FloatType>::max();
        }

        if (tMin >= 0.0)
        {
            return true;
        }
    }

    return false;
}

boundingBox3 sphere3::boundingBoxLocal() const
{
    vector3 r(radius, radius, radius);
    return boundingBox3(center - r, center + r);
}

vector3 sphere3::closestNormalLocal(const vector3& otherPoint) const
{
    if (center.isSimilar(otherPoint)) 
    {
        return vector3(1.0, 0.0, 0.0);
    }
    else
    {
        return (otherPoint - center).normalized();
    }
}

surfaceRayIntersection3 sphere3::closestIntersectionLocal(
        const ray3& ray) const
{
    surfaceRayIntersection3 intersection;
    vector3 r = ray.origin - center;
    FloatType b = ray.direction.dot(r);
    FloatType c = r.lengthSquared() - radius*radius;
    FloatType d = b * b - c;

    if (d > 0.0)
    {
        d = std::sqrt(d);
        FloatType tMin = -b - d;
        FloatType tMax = -b + d;

        if (tMin < 0.0)
        {
            tMin = tMax;
            tMax = std::numeric_limits<FloatType>::max();
        }

        if (tMin < 0.0) 
        {
            intersection.isIntersecting = false;
        } 
        else
        {
            intersection.isIntersecting = true;
            intersection.t = tMin;
            intersection.point = ray.origin + tMin * ray.direction;
            intersection.normal = (intersection.point - center).normalized();
        }
    } 
    else 
    {
        intersection.isIntersecting = false;
    }

    return intersection;
}


