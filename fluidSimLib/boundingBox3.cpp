//---boundingBox3.cpp

#include "boundingBox3.h"
#include <algorithm>

boundingBox3::boundingBox3()
{
    reset();
}

boundingBox3::boundingBox3(const vector3& lower, const vector3& upper)
{
    lowerCorner = lower;
    upperCorner = upper;
}

void boundingBox3::reset()
{
    lowerCorner.x = std::numeric_limits<double>::max();
    lowerCorner.y = std::numeric_limits<double>::max();
    lowerCorner.z = std::numeric_limits<double>::max();
    upperCorner.x = -std::numeric_limits<double>::max();
    upperCorner.y = -std::numeric_limits<double>::max();
    upperCorner.z = -std::numeric_limits<double>::max();
}


vector3 boundingBox3::corner(size_t idx) const
{
    vector3 result;
    if (idx & 1)
    {
        result.x = upperCorner.x;
    }
    else
    {
        result.x = lowerCorner.x;
    }
    //
    if (idx & 2)
    {
        result.y = upperCorner.y;
    }
    else
    {
        result.y = lowerCorner.y;
    }
    //
    if (idx & 4)
    {
        result.z = upperCorner.z;
    }
    else
    {
        result.z = lowerCorner.z;
    }
    return result;
}

double boundingBox3::width( ) const
{
    return upperCorner.x - lowerCorner.x;
}

double boundingBox3::height( ) const
{
    return upperCorner.y - lowerCorner.y;
}

double boundingBox3::depth( ) const
{
    return upperCorner.z - lowerCorner.z;
}

vector3 boundingBox3::midPoint( ) const
{
    return (upperCorner + lowerCorner) * 0.5;
}

bool boundingBox3::contains(const vector3& point) const
{
    if (upperCorner.x < point.x || lowerCorner.x > point.x)
    {
        return false;
    }

    if (upperCorner.y < point.y || lowerCorner.y > point.y)
    {
        return false;
    }

    if (upperCorner.z < point.z || lowerCorner.z > point.z)
    {
        return false;
    }

    return true;
}

bool boundingBox3::intersects(const ray3& ray) const
{
    double tMin = 0;
    double tMax = std::numeric_limits<double>::max();
    const vector3& rayInvDir = ray.direction.rdiv(1);

    for (int i = 0; i < 3; ++i)
    {
        double tNear = (lowerCorner[i] - ray.origin[i]) * rayInvDir[i];
        double tFar = (upperCorner[i] - ray.origin[i]) * rayInvDir[i];

        if (tNear > tFar) std::swap(tNear, tFar);
        tMin = tNear > tMin ? tNear : tMin;
        tMax = tFar < tMax ? tFar : tMax;

        if (tMin > tMax) return false;
    }

    return true;
}

boundingBoxRayIntersection3 boundingBox3::closestIntersection(const ray3& ray) const
{
    boundingBoxRayIntersection3 intersection;

    double tMin = 0.0;
    double tMax = std::numeric_limits<double>::max();
    const vector3& rayInvDir = ray.direction.rdiv(1);

    for (int i = 0; i < 3; ++i)
    {
        double tNear = (lowerCorner[i] - ray.origin[i]) * rayInvDir[i];
        double tFar = (upperCorner[i] - ray.origin[i]) * rayInvDir[i];

        if (tNear > tFar) std::swap(tNear, tFar);
        tMin = tNear > tMin ? tNear : tMin;
        tMax = tFar < tMax ? tFar : tMax;

        if (tMin > tMax)
        {
            intersection.isIntersecting = false;
            return intersection;
        }
    }

    intersection.isIntersecting = true;

    if (contains(ray.origin))
    {
        intersection.tNear = tMax;
        intersection.tFar = std::numeric_limits<double>::max();
    }
    else
    {
        intersection.tNear = tMin;
        intersection.tFar = tMax;
    }

    return intersection;
}

boundingBox3& boundingBox3::operator=(const boundingBox3& v)
{
    lowerCorner = v.lowerCorner;
    upperCorner = v.upperCorner;
    return (*this);
}