//---vector3.cpp

#include "vector3.h"

#include <cmath>
#include <algorithm>

// Constructors
 
vector3::vector3() :
    x(0),
    y(0),
    z(0) 
{
}

 
vector3::vector3(FloatType newX, FloatType newY, FloatType newZ) :
    x(newX),
    y(newY),
    z(newZ) 
{
}


 
vector3::vector3(const vector3& v) :
    x(v.x),
    y(v.y),
    z(v.z) 
{
}

// Basic setters
 
void vector3::set(FloatType s) 
{
    x = s;
    y = s;
    z = s;
}

 
void vector3::set(FloatType newX, FloatType newY, FloatType newZ) 
{
    x = newX;
    y = newY;
    z = newZ;
}

 
void vector3::set(const vector3& v) 
{
    x = v.x;
    y = v.y;
    z = v.z;
}

 
void vector3::setZero() 
{
    x = y = z = 0.0;
}

 
void vector3::normalize() 
{
    FloatType l = length();
    x /= l;
    y /= l;
    z /= l;
}

// Binary operators: new instance = this (+) v
 
vector3 vector3::add(FloatType v) const 
{
    return vector3(x + v, y + v, z + v);
}

 
vector3 vector3::add(const vector3& v) const
{
    return vector3(x + v.x, y + v.y, z + v.z);
}

 
vector3 vector3::sub(FloatType v) const 
{
    return vector3(x - v, y - v, z - v);
}

 
vector3 vector3::sub(const vector3& v) const
{
    return vector3(x - v.x, y - v.y, z - v.z);
}

 
vector3 vector3::mul(FloatType v) const 
{
    return vector3(x * v, y * v, z * v);
}

 
vector3 vector3::mul(const vector3& v) const {
    return vector3(x * v.x, y * v.y, z * v.z);
}

 
vector3 vector3::div(FloatType v) const {
    return vector3(x / v, y / v, z / v);
}

 
vector3 vector3::div(const vector3& v) const {
    return vector3(x / v.x, y / v.y, z / v.z);
}

 
FloatType vector3::dot(const vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

 
vector3 vector3::cross(const vector3& v) const {
    return vector3(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y);
}

vector3 vector3::rdiv(FloatType v) const
{
    return vector3(v / x, v / y, v / z);
}

vector3 vector3::rdiv(const vector3& v) const
{
    return vector3(v.x / x, v.y / y, v.z / z);
}
 
FloatType vector3::sum() const 
{
    return x + y + z;
}

 
FloatType vector3::avg() const 
{
    return (x + y + z)/(FloatType)3.0;
}

 
FloatType vector3::min() const 
{
    return std::min(std::min(x, y), z);
}

 
FloatType vector3::max() const 
{
    return std::max(std::max(x, y), z);
}


 
vector3 vector3::normalized() const 
{
    FloatType l = length();
    return vector3(x / l, y / l, z / l);
}

 
FloatType vector3::length() const 
{
    return std::sqrt(x * x + y * y + z * z);
}

 
FloatType vector3::lengthSquared() const {
    return x * x + y * y + z * z;
}

 
FloatType vector3::distanceTo(const vector3& other) const 
{
    return sub(other).length();
}

 
FloatType vector3::distanceSquaredTo(const vector3& other) const 
{
    return sub(other).lengthSquared();
}

 
vector3 vector3::reflected(const vector3& normal) const
{
    // this - 2(this.n)n
    return sub(normal.mul(2 * dot(normal)));
}

 
vector3 vector3::projected(const vector3& normal) const 
{
    // this - this.n n
    return sub(normal.mul(dot(normal)));
}

 
void vector3::tangential( vector3& a, vector3& b ) const 
{
    a = ((std::fabs(y) > 0 || std::fabs(z) > 0)
            ? vector3(1, 0, 0)
            : vector3(0, 1, 0)).cross(*this).normalized();
    b = cross(a);
   
}
 
bool vector3::isEqual(const vector3& other) const
{
    return x == other.x && y == other.y && z == other.z;
}

bool vector3::isSimilar( const vector3& other, FloatType eps ) const
{
    return (std::fabs(x - other.x) < eps) &&
        (std::fabs(y - other.y) < eps) &&
        (std::fabs(z - other.z) < eps);
}

FloatType& vector3::operator[](SizeType i)
{
    return (&x)[i];
}

const FloatType& vector3::operator[](SizeType i) const
{
    return (&x)[i];
}

vector3& vector3::operator=(const vector3& v) 
{
    set(v);
    return (*this);
}

 
vector3& vector3::operator+=(FloatType v) 
{
    x += v;
    y += v;
    z += v;
    return (*this);
}

 
vector3& vector3::operator+=(const vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return (*this);
}

 
vector3& vector3::operator-=(FloatType v) 
{
    x -= v;
    y -= v;
    z -= v;
    return (*this);
}

 
vector3& vector3::operator-=(const vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return (*this);
}

 
vector3& vector3::operator*=(FloatType v)
{
    x *= v;
    y *= v;
    z *= v;
    return (*this);
}

 
vector3& vector3::operator*=(const vector3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return (*this);
}

 
vector3& vector3::operator/=(FloatType v)
{
    x /= v;
    y /= v;
    z /= v;
    return (*this);
}

 
vector3& vector3::operator/=(const vector3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return (*this);
}

 
bool vector3::operator==(const vector3& v) const
{
    return isEqual(v);
}

 
bool vector3::operator!=(const vector3& v) const 
{
    return !isEqual(v);
}

vector3 operator+(const vector3& a)
{
    return a;
}

vector3 operator-(const vector3& a)
{
    return vector3( -a.x, -a.y, -a.z );
}

vector3 operator+(const vector3& a, FloatType b)
{
    return a.add( b );
}

vector3 operator+(FloatType a, const vector3& b)
{
    return b.add( a );
}

vector3 operator+(const vector3& a, const vector3& b)
{
    return a.add( b );
}

vector3 operator-(const vector3& a, FloatType b)
{
    return a.sub( b );
}

vector3 operator-(FloatType a, const vector3& b)
{
    return vector3( a - b.x, a - b.y, a - b.z );
}

vector3 operator-(const vector3& a, const vector3& b)
{
    return a.sub( b );
}

vector3 operator*(const vector3& a, FloatType b)
{
    return a.mul( b );
}

vector3 operator*( FloatType a, const vector3& b)
{
    return b.mul( a );
}

vector3 operator*(const vector3& a, const vector3& b)
{
    return a.mul( b );
}

vector3 operator/(const vector3& a, FloatType b)
{
    return a.div( b );
}

vector3 operator/(FloatType a, const vector3& b)
{
    return vector3( a/b.x, a/b.y, a/b.z );
}

vector3 operator/(const vector3& a, const vector3& b)
{
    return a.div( b );
}


