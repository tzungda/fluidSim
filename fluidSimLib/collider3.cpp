//---collider3.cpp

#include <cmath>
#include <algorithm>
#include "collider3.h"


collider3::collider3() 
{
}

collider3::~collider3() 
{
}

void collider3::resolveCollision(
    FloatType radius,
    FloatType restitutionCoefficient,
    vector3* newPosition,
    vector3* newVelocity)
{
    colliderQueryResult colliderPoint;

    getClosestPoint(mSurface, *newPosition, &colliderPoint);

    // check if the new position is penetrating the surface
    if (isPenetrating(colliderPoint, *newPosition, radius))
    {
        // target point is the closest non-penetrating position from the
        // new position.
        vector3 targetNormal = colliderPoint.normal;
        vector3 targetPoint = colliderPoint.point + radius * targetNormal;
        vector3 colliderVelAtTargetPoint = colliderPoint.velocity;

        // get new candidate relative velocity from the target point.
        vector3 relativeVel = *newVelocity - colliderVelAtTargetPoint;
        FloatType normalDotRelativeVel = targetNormal.dot(relativeVel);
        vector3 relativeVelN = normalDotRelativeVel * targetNormal;
        vector3 relativeVelT = relativeVel - relativeVelN;

        // check if the velocity is facing opposite direction of the surface
        // normal
        if (normalDotRelativeVel < 0.0)
        {
            // apply restitution coefficient to the surface normal component of
            // the velocity
            vector3 deltaRelativeVelN
                = (-restitutionCoefficient - (FloatType)1.0) * relativeVelN;
            relativeVelN *= -restitutionCoefficient;

            // apply friction to the tangential component of the velocity
            // from Bridson et al., Robust Treatment of Collisions, Contact and
            // friction for Cloth Animation, 2002
            // http://graphics.stanford.edu/papers/cloth-sig02/cloth.pdf
            if (relativeVelT.lengthSquared() > 0.0) 
            {
                FloatType frictionScale
                    = (FloatType)std::max(
                        1.0
                        - mFrictionCoeffient
                        * deltaRelativeVelN.length()
                        / relativeVelT.length(), 0.0);
                relativeVelT *= frictionScale;
            }

            // reassemble the components
            *newVelocity = relativeVelN + relativeVelT + colliderVelAtTargetPoint;
        }

        // geometric fix
        *newPosition = targetPoint;
    }
}

FloatType collider3::frictionCoefficient() const 
{
    return mFrictionCoeffient;
}

void collider3::setFrictionCoefficient(FloatType newFrictionCoeffient) 
{
    mFrictionCoeffient = std::max<FloatType>(newFrictionCoeffient, 0.0);
}

const surface3Ptr& collider3::surface() const 
{
    return mSurface;
}

void collider3::setSurface(const surface3Ptr& newSurface)
{
    mSurface = newSurface;
}

void collider3::getClosestPoint(
    const surface3Ptr& surface,
    const vector3& queryPoint,
    colliderQueryResult* result) const 
{
    result->distance = surface->closestDistance(queryPoint);
    result->point = surface->closestPoint(queryPoint);
    result->normal = surface->closestNormal(queryPoint);
    result->velocity = velocityAt(queryPoint);
}

bool collider3::isPenetrating(
    const colliderQueryResult& colliderPoint,
    const vector3& position,
    FloatType radius) 
{
    return (position - colliderPoint.point).dot(colliderPoint.normal) < 0.0 ||
        colliderPoint.distance < radius;
}

void collider3::update(
    FloatType currentTimeInSeconds,
    FloatType timeIntervalInSeconds) 
{
    if (mOnUpdateCallback) {
        mOnUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
    }
}

void collider3::setOnBeginUpdateCallback(
    const onBeginUpdateCallback& callback)
{
    mOnUpdateCallback = callback;
}


