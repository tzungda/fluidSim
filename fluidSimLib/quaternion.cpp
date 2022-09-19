//---quaternion.cpp

#include <cmath>
#include "quaternion.h"

quaternion::quaternion()
{
    setIdentity();
}

quaternion::quaternion(FloatType newW, FloatType newX, FloatType newY, FloatType newZ)
{
    set(newW, newX, newY, newZ);
}

quaternion::quaternion(const matrix33& m33)
{
    set(m33);
}

void quaternion::set(FloatType newW, FloatType newX, FloatType newY, FloatType newZ)
{
    w = newW, x = newX, y = newY, z = newZ;
}

void quaternion::set(const matrix33& m33)
{
    static const FloatType eps = std::numeric_limits<FloatType>::epsilon();
    static const FloatType quater = static_cast<FloatType>(0.25);

    FloatType onePlusTrace = m33.trace() + (FloatType)1.0;

    if (onePlusTrace > eps) 
    {
        FloatType S = std::sqrt(onePlusTrace) * (FloatType)2.0;
        w = quater * S;
        x = (m33(2, 1) - m33(1, 2)) / S;
        y = (m33(0, 2) - m33(2, 0)) / S;
        z = (m33(1, 0) - m33(0, 1)) / S;
    }
    else if (m33(0, 0) > m33(1, 1) && m33(0, 0) > m33(2, 2))
    {
        FloatType S = std::sqrt((FloatType)1.0 + m33(0, 0) - m33(1, 1) - m33(2, 2)) * (FloatType)2.0;
        w = (m33(2, 1) - m33(1, 2)) / S;
        x = quater * S;
        y = (m33(0, 1) + m33(1, 0)) / S;
        z = (m33(0, 2) + m33(2, 0)) / S;
    }
    else if (m33(1, 1) > m33(2, 2)) 
    {
        FloatType S = std::sqrt((FloatType)1.0 + m33(1, 1) - m33(0, 0) - m33(2, 2)) * (FloatType)2.0;
        w = (m33(0, 2) - m33(2, 0)) / S;
        x = (m33(0, 1) + m33(1, 0)) / S;
        y = quater * S;
        z = (m33(1, 2) + m33(2, 1)) / S;
    }
    else 
    {
        FloatType S = std::sqrt((FloatType)1.0 + m33(2, 2) - m33(0, 0) - m33(1, 1)) * (FloatType)2.0;
        w = (m33(1, 0) - m33(0, 1)) / S;
        x = (m33(0, 2) + m33(2, 0)) / S;
        y = (m33(1, 2) + m33(2, 1)) / S;
        z = quater * S;
    }
}

void quaternion::setIdentity()
{
    set(1.0, 0.0, 0.0, 0.0);
}

matrix33 quaternion::matrix3() const
{
    FloatType _2xx = (FloatType)2.0 * x * x;
    FloatType _2yy = (FloatType)2.0 * y * y;
    FloatType _2zz = (FloatType)2.0 * z * z;
    FloatType _2xy = (FloatType)2.0 * x * y;
    FloatType _2xz = (FloatType)2.0 * x * z;
    FloatType _2xw = (FloatType)2.0 * x * w;
    FloatType _2yz = (FloatType)2.0 * y * z;
    FloatType _2yw = (FloatType)2.0 * y * w;
    FloatType _2zw = (FloatType)2.0 * z * w;

    matrix33 m(
        (FloatType)1.0 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw,
        _2xy + _2zw, (FloatType)1.0 - _2zz - _2xx, _2yz - _2xw,
        _2xz - _2yw, _2yz + _2xw, (FloatType)1.0 - _2yy - _2xx);

    return m;
}


quaternion quaternion::inverse() const
{
    FloatType denom = w * w + x * x + y * y + z * z;
    return quaternion(w / denom, -x / denom, -y / denom, -z / denom);
}

quaternion quaternion::mul(const quaternion& other) const
{
    return quaternion(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w);
}

void quaternion::imul(const quaternion& other)
{
    *this = mul(other);
}

quaternion& quaternion::operator=(const quaternion& other)
{
    set(other.w, other.x, other.y, other.z);
    return *this;
}

quaternion& quaternion::operator*=(const quaternion& other)
{
    imul(other);
    return *this;
}


