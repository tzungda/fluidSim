//---box3.cpp

#include "box3.h"
#include "mathUtil.h"
#include "plane3.h"

box3::box3(
    const transform3& transform,
    bool isNormalFlipped)
    : surface3(transform, isNormalFlipped) 
    {
}

box3::box3(
    const vector3& lowerCorner,
    const vector3& upperCorner,
    const transform3& transform,
    bool isNormalFlipped) :
    box3(boundingBox3(lowerCorner, upperCorner), transform, isNormalFlipped) 
{
}

box3::box3(
    const boundingBox3& boundingBox,
    const transform3& transform,
    bool isNormalFlipped)
    : surface3(transform, isNormalFlipped)
    , bound(boundingBox)
{
}

box3::box3(const box3& other) : surface3(other), bound(other.bound)
{
}

vector3 box3::closestPointLocal(const vector3& otherPoint) const
{
    if (bound.contains(otherPoint))
    {
        plane3 planes[6] = {
            plane3(vector3(1, 0, 0), bound.upperCorner),
            plane3(vector3(0, 1, 0), bound.upperCorner),
            plane3(vector3(0, 0, 1), bound.upperCorner),
            plane3(vector3(-1, 0, 0), bound.lowerCorner),
            plane3(vector3(0, -1, 0), bound.lowerCorner),
            plane3(vector3(0, 0, -1), bound.lowerCorner)
        };

        vector3 result = planes[0].closestPoint(otherPoint);
        double distanceSquared = result.distanceSquaredTo(otherPoint);

        for (int i = 1; i < 6; ++i) 
        {
            vector3 localResult = planes[i].closestPoint(otherPoint);
            double localDistanceSquared
                = localResult.distanceSquaredTo(otherPoint);

            if (localDistanceSquared < distanceSquared)
            {
                result = localResult;
                distanceSquared = localDistanceSquared;
            }
        }

        return result;
    }
    else 
    {
        return mathUtil::clamp(
            otherPoint,
            bound.lowerCorner,
            bound.upperCorner);
    }
}

vector3 box3::closestNormalLocal(const vector3& otherPoint) const 
{
    plane3 planes[6] = {
        plane3(vector3(1, 0, 0), bound.upperCorner),
        plane3(vector3(0, 1, 0), bound.upperCorner),
        plane3(vector3(0, 0, 1), bound.upperCorner),
        plane3(vector3(-1, 0, 0), bound.lowerCorner),
        plane3(vector3(0, -1, 0), bound.lowerCorner),
        plane3(vector3(0, 0, -1), bound.lowerCorner)
    };

    if (bound.contains(otherPoint)) 
    {
        vector3 closestNormal = planes[0].normal;
        vector3 closestPoint = planes[0].closestPoint(otherPoint);
        double minDistanceSquared = (closestPoint - otherPoint).lengthSquared();

        for (int i = 1; i < 6; ++i)
        {
            vector3 localClosestPoint = planes[i].closestPoint(otherPoint);
            double localDistanceSquared
                = (localClosestPoint - otherPoint).lengthSquared();

            if (localDistanceSquared < minDistanceSquared)
            {
                closestNormal = planes[i].normal;
                minDistanceSquared = localDistanceSquared;
            }
        }

        return closestNormal;
    }
    else
    {
        vector3 closestPoint = mathUtil::clamp(
            otherPoint,
            bound.lowerCorner,
            bound.upperCorner);
        vector3 closestPointToInputPoint = otherPoint - closestPoint;
        vector3 closestNormal = planes[0].normal;
        double maxCosineAngle = closestNormal.dot(closestPointToInputPoint);

        for (int i = 1; i < 6; ++i)
        {
            double cosineAngle
                = planes[i].normal.dot(closestPointToInputPoint);

            if (cosineAngle > maxCosineAngle) 
            {
                closestNormal = planes[i].normal;
                maxCosineAngle = cosineAngle;
            }
        }

        return closestNormal;
    }
}

bool box3::intersectsLocal(const ray3& ray) const
{
    return bound.intersects(ray);
}

surfaceRayIntersection3 box3::closestIntersectionLocal(
    const ray3& ray) const
{
    surfaceRayIntersection3 intersection;
    boundingBoxRayIntersection3 bbRayIntersection
        = bound.closestIntersection(ray);
    intersection.isIntersecting = bbRayIntersection.isIntersecting;
    if (intersection.isIntersecting) 
    {
        intersection.t = bbRayIntersection.tNear;
        intersection.point = ray.pointAt(bbRayIntersection.tNear);
        intersection.normal = box3::closestNormal(intersection.point);
    }

    return intersection;
}

boundingBox3 box3::boundingBoxLocal() const
{
    return bound;
}

