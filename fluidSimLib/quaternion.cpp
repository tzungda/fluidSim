//---quaternion.cpp

#include <cmath>
#include "quaternion.h"

quaternion::quaternion()
{
    setIdentity();
}

quaternion::quaternion(double newW, double newX, double newY, double newZ)
{
    set(newW, newX, newY, newZ);
}

quaternion::quaternion(const matrix33& m33)
{
    set(m33);
}

void quaternion::set(double newW, double newX, double newY, double newZ)
{
    w = newW, x = newX, y = newY, z = newZ;
}

void quaternion::set(const matrix33& m33)
{
    static const double eps = std::numeric_limits<double>::epsilon();
    static const double quater = static_cast<double>(0.25);

    double onePlusTrace = m33.trace() + 1.0;

    if (onePlusTrace > eps) 
    {
        double S = std::sqrt(onePlusTrace) * 2.0;
        w = quater * S;
        x = (m33(2, 1) - m33(1, 2)) / S;
        y = (m33(0, 2) - m33(2, 0)) / S;
        z = (m33(1, 0) - m33(0, 1)) / S;
    }
    else if (m33(0, 0) > m33(1, 1) && m33(0, 0) > m33(2, 2))
    {
        double S = std::sqrt(1.0 + m33(0, 0) - m33(1, 1) - m33(2, 2)) * 2.0;
        w = (m33(2, 1) - m33(1, 2)) / S;
        x = quater * S;
        y = (m33(0, 1) + m33(1, 0)) / S;
        z = (m33(0, 2) + m33(2, 0)) / S;
    }
    else if (m33(1, 1) > m33(2, 2)) 
    {
        double S = std::sqrt(1.0 + m33(1, 1) - m33(0, 0) - m33(2, 2)) * 2.0;
        w = (m33(0, 2) - m33(2, 0)) / S;
        x = (m33(0, 1) + m33(1, 0)) / S;
        y = quater * S;
        z = (m33(1, 2) + m33(2, 1)) / S;
    }
    else 
    {
        double S = std::sqrt(1.0 + m33(2, 2) - m33(0, 0) - m33(1, 1)) * 2.0;
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
    double _2xx = 2.0 * x * x;
    double _2yy = 2.0 * y * y;
    double _2zz = 2.0 * z * z;
    double _2xy = 2.0 * x * y;
    double _2xz = 2.0 * x * z;
    double _2xw = 2.0 * x * w;
    double _2yz = 2.0 * y * z;
    double _2yw = 2.0 * y * w;
    double _2zw = 2.0 * z * w;

    matrix33 m(
        1.0 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw,
        _2xy + _2zw, 1.0 - _2zz - _2xx, _2yz - _2xw,
        _2xz - _2yw, _2yz + _2xw, 1.0 - _2yy - _2xx);

    return m;
}


quaternion quaternion::inverse() const
{
    double denom = w * w + x * x + y * y + z * z;
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


