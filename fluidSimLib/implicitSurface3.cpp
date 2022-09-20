//---implicitSurface3.cpp

#include <cmath>
#include "implicitSurface3.h"

implicitSurface3::implicitSurface3( const transform3& transform_, bool isNormalFlipped_)
    : surface3(transform_, isNormalFlipped_)
{
}

implicitSurface3::implicitSurface3(const implicitSurface3& other) : surface3(other) 
{
}

implicitSurface3::~implicitSurface3() 
{
}

double implicitSurface3::signedDistance(const vector3& otherPoint) const
{
    return signedDistanceLocal(transform.toLocal(otherPoint));
}

double implicitSurface3::closestDistanceLocal( const vector3& otherPoint) const
{
    return std::fabs(signedDistanceLocal(otherPoint));
}

