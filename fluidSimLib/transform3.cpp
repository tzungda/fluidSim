//---transform3.cpp

#include "transform3.h"

transform3::transform3()
{
}

transform3::transform3(const vector3& translation, const quaternion& orientation)
{
    setTranslation(translation);
    setOrientation(orientation);
}

const vector3& transform3::translation() const
{
    return mTranslation;
}

void transform3::setTranslation(const vector3& translation)
{
    mTranslation = translation;
}

const quaternion& transform3::orientation() const
{
    return mOrientation;
}

void transform3::setOrientation(const quaternion& orientation)
{
    mOrientation = orientation;
    mOrientationMat3 = orientation.matrix3();
    mInverseOrientationMat3 = orientation.inverse().matrix3();
}

vector3 transform3::toLocal(const vector3& pointInWorld) const
{
    return mInverseOrientationMat3 * (pointInWorld - mTranslation);
}

ray3 transform3::toLocal(const ray3& rayInWorld) const
{
    return ray3(
        toLocal(rayInWorld.origin),
        toLocalDirection(rayInWorld.direction));
}

vector3 transform3::toWorld(const vector3& pointInLocal) const
{
    return (mOrientationMat3 * pointInLocal) + mTranslation;
}

boundingBox3 transform3::toWorld(const boundingBox3& bboxInLocal) const
{
    boundingBox3 bboxInWorld = bboxInLocal;
    for (int i = 0; i < 8; ++i) 
    {
        vector3 cornerInWorld = toWorld(bboxInLocal.corner(i));
        bboxInWorld.lowerCorner
            = vector3(std::min(bboxInWorld.lowerCorner.x, cornerInWorld.x), std::min(bboxInWorld.lowerCorner.y, cornerInWorld.y), std::min(bboxInWorld.lowerCorner.z, cornerInWorld.z));
        bboxInWorld.upperCorner
            = vector3(std::max(bboxInWorld.upperCorner.x, cornerInWorld.x), std::max(bboxInWorld.upperCorner.y, cornerInWorld.y), std::max(bboxInWorld.upperCorner.z, cornerInWorld.z));
    }
    return bboxInWorld;
}

vector3 transform3::toLocalDirection(const vector3& dirInWorld) const
{
    return mInverseOrientationMat3 * dirInWorld;
}


vector3 transform3::toWorldDirection(const vector3& dirInLocal) const
{
    return mOrientationMat3 * dirInLocal;
}


