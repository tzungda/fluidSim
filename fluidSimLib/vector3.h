//---vector3.h
#pragma once

//#include <algorithm>  
#include <limits>
//#include <tuple>
#include "common.h"

class vector3
{
public:

    FloatType x;

    FloatType y;

    FloatType z;


    vector3();

    vector3(FloatType x, FloatType y, FloatType z);

    vector3(const vector3& v);

    void set(FloatType s);

    void set(FloatType x, FloatType y, FloatType z);

    void set(const vector3& v);

    void setZero();

    void normalize();

    vector3 add(FloatType v) const;

    vector3 add( const vector3& v) const;

    vector3 sub(FloatType v) const;

    vector3 sub(const vector3& v) const;

    vector3 mul(FloatType v) const;

    vector3 mul(const vector3& v) const;

    vector3 div(FloatType v) const;

    vector3 div(const vector3& v) const;

    FloatType dot(const vector3& v) const;

    vector3 cross(const vector3& v) const;

    vector3 rdiv(FloatType v) const;

    vector3 rdiv(const vector3& v) const;


    //! Returns the sum of all the components (i.e. x + y + z).
    FloatType sum() const;

    //! Returns the average of all the components.
    FloatType avg() const;

    //! Returns the minimum value among x, y, and z.
    FloatType min() const;

    //! Returns the maximum value among x, y, and z.
    FloatType max() const;


    //! Returns normalized vector.
    vector3 normalized() const;

    //! Returns the length of the vector.
    FloatType length() const;

    //! Returns the squared length of the vector.
    FloatType lengthSquared() const;

    //! Returns the distance to the other vector.
    FloatType distanceTo(const vector3& other) const;

    //! Returns the squared distance to the other vector.
    FloatType distanceSquaredTo(const vector3& other) const;

    //! Returns the reflection vector3 to the surface with given surface normal.
    vector3 reflected(const vector3& normal) const;

    //! Returns the projected vector3 to the surface with given surface normal.
    vector3 projected(const vector3& normal) const;

    //! Returns the tangential vector3 for this vector.
    void tangential( vector3& a, vector3& b ) const;

    //! Returns true if \p other is the same as this vector.
    bool isEqual(const vector3& other) const;

    bool isSimilar( const vector3& other, FloatType eps = 0.000001f ) const;

    FloatType& operator[](size_t i);
    const FloatType& operator[](size_t i) const;

    //! SeFloatType x and y with other vector3 \p pt.
    vector3& operator=(const vector3& v);

    //! Computes this += (v, v)
    vector3& operator+=(FloatType v);

    //! Computes this += (v.x, v.y)
    vector3& operator+=(const vector3& v);

    //! Computes this -= (v, v)
    vector3& operator-=(FloatType v);

    //! Computes this -= (v.x, v.y)
    vector3& operator-=(const vector3& v);

    //! Computes this *= (v, v)
    vector3& operator*=(FloatType v);

    //! Computes this *= (v.x, v.y)
    vector3& operator*=(const vector3& v);

    //! Computes this /= (v, v)
    vector3& operator/=(FloatType v);

    //! Computes this /= (v.x, v.y)
    vector3& operator/=(const vector3& v);

    //! Returns true if \p other is the same as this vector.
    bool operator==(const vector3& v) const;

    //! Returns true if \p other is the noFloatType same as this vector.
    bool operator!=(const vector3& v) const;

   
};

vector3 operator+(const vector3& a);

vector3 operator-(const vector3& a);

vector3 operator+(const vector3& a, FloatType b);

vector3 operator+(FloatType a, const vector3& b);

vector3 operator+(const vector3& a, const vector3& b);

vector3 operator-(const vector3& a, FloatType b);

vector3 operator-(FloatType a, const vector3& b);

vector3 operator-(const vector3& a, const vector3& b);

vector3 operator*(const vector3& a, FloatType b);

vector3 operator*( FloatType a, const vector3& b);

vector3 operator*(const vector3& a, const vector3& b);

vector3 operator/(const vector3& a, FloatType b);

vector3 operator/(FloatType a, const vector3& b);

vector3 operator/(const vector3& a, const vector3& b);


