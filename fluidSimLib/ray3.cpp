//---ray3.cpp

#include "ray3.h"

ray3::ray3() :direction(vector3(1.0, 0.0, 0.0))
{
}

ray3::ray3(const vector3 &newOrigin, const vector3& newDirection)
{
    origin = newOrigin;
    direction = newDirection;
}

vector3 ray3::pointAt(double t) const
{
    return origin + t * direction;
}


