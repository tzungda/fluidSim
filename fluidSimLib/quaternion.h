//---quaternion.h
#ifndef quaternion_H
#define quaternion_H

#include "matrix33.h"

class quaternion
{
public:
    double w;
    double x;
    double y;
    double z;

    quaternion();

    quaternion(double newW, double newX, double newY, double newZ);

    quaternion(const matrix33& m33);

    void set(double newW, double newX, double newY, double newZ);
    void set(const matrix33& m33);

    void setIdentity();

    matrix33 matrix3() const;
    quaternion inverse() const;

    quaternion mul(const quaternion& other) const;
    void imul(const quaternion& other);

    quaternion& operator=(const quaternion& other);

    quaternion& operator*=(const quaternion& other);
};

#endif

