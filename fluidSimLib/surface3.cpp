//---surface3.cpp

#include "surface3.h"

surface3::surface3(const transform3& _transform, bool _isNormalFlipped) :
    transform(_transform), isNormalFlipped(_isNormalFlipped)
{
}

surface3::surface3(const surface3& other) :
    transform(other.transform), isNormalFlipped(other.isNormalFlipped)
{
}

surface3::~surface3()
{
}

vector3 surface3::closestPoint(const vector3& otherPoint) const
{
    return transform.toWorld(closestPointLocal(transform.toLocal(otherPoint)));
}

boundingBox3 surface3::boundingBox() const
{
    return transform.toWorld(boundingBoxLocal());
}

bool surface3::intersects(const ray3& ray) const
{
    return intersectsLocal(transform.toLocal(ray));
}

double surface3::closestDistance(const vector3& otherPoint) const
{
    return closestDistanceLocal(transform.toLocal(otherPoint));
}

surfaceRayIntersection3 surface3::closestIntersection(const ray3& ray) const
{
    surfaceRayIntersection3 result = closestIntersectionLocal(transform.toLocal(ray));
    result.point = transform.toWorld(result.point);
    result.normal = transform.toWorldDirection(result.normal);
    result.normal *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

vector3 surface3::closestNormal(const vector3& otherPoint) const
{
    vector3 result = transform.toWorldDirection(
        closestNormalLocal(transform.toLocal(otherPoint)));
    result *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

bool surface3::intersectsLocal(const ray3& rayLocal) const
{
    auto result = closestIntersectionLocal(rayLocal);
    return result.isIntersecting;
}

double surface3::closestDistanceLocal(const vector3& otherPointLocal) const
{
    return otherPointLocal.distanceTo(closestPointLocal(otherPointLocal));
}


