//---boundingBox3.h
#ifndef boundingBox3_H
#define boundingBox3_H

#include "vector3.h"
#include "ray3.h"

struct boundingBoxRayIntersection3
{

    bool isIntersecting = false;


    double tNear = std::numeric_limits<double>::max();


    double tFar = std::numeric_limits<double>::max();
};

class boundingBox3
{
public:
    vector3 lowerCorner;
    vector3 upperCorner;

    boundingBox3();
    boundingBox3(const vector3& lower, const vector3& upper);

    void reset();

    vector3 corner(size_t idx) const;

    double width( ) const;
    double height( ) const;
    double depth( ) const;
    vector3 midPoint( ) const;

    bool contains(const vector3& point) const;

    bool intersects(const ray3& ray) const;

    boundingBoxRayIntersection3 closestIntersection(const ray3& ray) const;

    boundingBox3& operator=(const boundingBox3& v);

};

#endif


