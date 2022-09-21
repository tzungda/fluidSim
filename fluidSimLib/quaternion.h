//---quaternion.h
#ifndef quaternion_H
#define quaternion_H

#include "matrix33.h"

class quaternion
{
public:
    FloatType w;
    FloatType x;
    FloatType y;
    FloatType z;

    quaternion();

    quaternion(FloatType newW, FloatType newX, FloatType newY, FloatType newZ);

    quaternion(const matrix33& m33);

    void set(FloatType newW, FloatType newX, FloatType newY, FloatType newZ);
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

