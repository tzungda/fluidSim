//---plane3.cpp

#include <cmath>
#include "plane3.h"


plane3::plane3(const transform3& transform_, bool isNormalFlipped_)
    : surface3(transform_, isNormalFlipped_)
{
}

plane3::plane3(
    const vector3& normal,
    const vector3& point,
    const transform3& transform_,
    bool isNormalFlipped_)
    : surface3(transform_, isNormalFlipped_)
    , normal(normal)
    , point(point) 
{
}

plane3::plane3(
    const vector3& point0,
    const vector3& point1,
    const vector3& point2,
    const transform3& transform_,
    bool isNormalFlipped_) : surface3(transform_, isNormalFlipped_) 
{
    normal = (point1 - point0).cross(point2 - point0).normalized();
    point = point0;
}

plane3::plane3(const plane3& other) :
    surface3(other),
    normal(other.normal),
    point(other.point)
{
}

vector3 plane3::closestPointLocal(const vector3& otherPoint) const
{
    vector3 r = otherPoint - point;
    return r - normal.dot(r) * normal + point;
}

vector3 plane3::closestNormalLocal(const vector3& otherPoint) const 
{
    return normal;
}

bool plane3::intersectsLocal(const ray3& ray) const 
{
    return std::fabs(ray.direction.dot(normal)) > 0.0;
}

surfaceRayIntersection3 plane3::closestIntersectionLocal( const ray3& ray) const
{
    surfaceRayIntersection3 intersection;

    FloatType dDotN = ray.direction.dot(normal);

    // Check if not parallel
    if (std::fabs(dDotN) > 0) 
    {
        FloatType t = normal.dot(point - ray.origin) / dDotN;
        if (t >= 0.0) 
        {
            intersection.isIntersecting = true;
            intersection.t = t;
            intersection.point = ray.pointAt(t);
            intersection.normal = normal;
        }
    }

    return intersection;
}

boundingBox3 plane3::boundingBoxLocal() const 
{
    static const FloatType eps = std::numeric_limits<FloatType>::epsilon();
    static const FloatType dmax = std::numeric_limits<FloatType>::max();

    if (std::fabs(normal.dot(vector3(1.0, 0.0, 0.0)) - 1.0) < eps) 
    {
        return boundingBox3(
            point - vector3(0.0, dmax, dmax),
            point + vector3(0.0, dmax, dmax));
    } 
    else if (std::fabs(normal.dot(vector3(0.0, 1.0, 0.0)) - 1.0) < eps) 
    {
        return boundingBox3(
            point - vector3(dmax, 0.0, dmax),
            point + vector3(dmax, 0.0, dmax));
    }
    else if (std::fabs(normal.dot(vector3(0.0, 0.0, 1.0)) - 1.0) < eps) 
    {
        return boundingBox3(
            point - vector3(dmax, dmax, 0.0),
            point + vector3(dmax, dmax, 0.0));
    }
    else
    {
        return boundingBox3(
            vector3(dmax, dmax, dmax),
            vector3(dmax, dmax, dmax));
    }
}



