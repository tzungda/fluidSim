//---transform3.h
#ifndef transform3_H
#define transform3_H

#include "vector3.h"
#include "quaternion.h"
#include "boundingBox3.h"
#include "ray3.h"

class transform3
{
public:
    transform3();

    transform3(const vector3& translation, const quaternion& orientation);

    const vector3& translation() const;

    void setTranslation(const vector3& translation);

    const quaternion& orientation() const;

    void setOrientation(const quaternion& orientation);

    vector3 toLocal(const vector3& pointInWorld) const;

    ray3 toLocal(const ray3& rayInWorld) const;

    vector3 toWorld(const vector3& pointInLocal) const;

    boundingBox3 toWorld( const boundingBox3& bboxInLocal ) const;

    vector3 toLocalDirection(const vector3& dirInWorld) const;
    vector3 toWorldDirection( const vector3& dirInLocal) const;

private:
    vector3 mTranslation;
    quaternion mOrientation;
    matrix33 mOrientationMat3;
    matrix33 mInverseOrientationMat3;
};

#endif

