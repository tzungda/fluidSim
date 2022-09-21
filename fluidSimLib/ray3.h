//---ray3.h
#ifndef ray3_H
#define ray3_H

#include "vector3.h"

class ray3
{
public:
    vector3 origin;
    vector3 direction;
public:
    ray3();

    ray3(const vector3 &newOrigin, const vector3& newDirection);

    vector3 pointAt(FloatType t) const;
};

#endif

