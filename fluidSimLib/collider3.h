//---collider3.h
#ifndef collider3_H
#define collider3_H

#include <memory>
#include <functional>

#include "surface3.h"


class collider3
{
public:
    typedef std::function<void(collider3*, double, double)>
        onBeginUpdateCallback;

    collider3();

    virtual ~collider3();

    virtual vector3 velocityAt(const vector3& point) const = 0;

    void resolveCollision(
        double radius,
        double restitutionCoefficient,
        vector3* position,
        vector3* velocity);

    double frictionCoefficient() const;

    void setFrictionCoefficient(double newFrictionCoeffient);

    const surface3Ptr& surface() const;

    void update(double currentTimeInSeconds, double timeIntervalInSeconds);

    void setOnBeginUpdateCallback(const onBeginUpdateCallback& callback);

protected:
    //! Internal query result structure.
    struct colliderQueryResult final {
        double distance;
        vector3 point;
        vector3 normal;
        vector3 velocity;
    };

    void setSurface(const surface3Ptr& newSurface);

    void getClosestPoint(
        const surface3Ptr& surface,
        const vector3& queryPoint,
        colliderQueryResult* result) const;

    bool isPenetrating(
        const colliderQueryResult& colliderPoint,
        const vector3& position,
        double radius);

private:
    surface3Ptr mSurface;
    double mFrictionCoeffient = 0.0;
    onBeginUpdateCallback mOnUpdateCallback;
};

typedef std::shared_ptr<collider3> collider3Ptr;

#endif

