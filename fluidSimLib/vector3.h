//---vector3.h
#pragma once

//#include <algorithm>  
#include <limits>
//#include <tuple>

class vector3
{
public:

    double x;

    double y;

    double z;


    vector3();

    vector3(double x, double y, double z);

    vector3(const vector3& v);

    void set(double s);

    void set(double x, double y, double z);

    void set(const vector3& v);

    void setZero();

    void normalize();

    vector3 add(double v) const;

    vector3 add( const vector3& v) const;

    vector3 sub(double v) const;

    vector3 sub(const vector3& v) const;

    vector3 mul(double v) const;

    vector3 mul(const vector3& v) const;

    vector3 div(double v) const;

    vector3 div(const vector3& v) const;

    double dot(const vector3& v) const;

    vector3 cross(const vector3& v) const;

    vector3 rdiv(double v) const;

    vector3 rdiv(const vector3& v) const;


    //! Returns the sum of all the components (i.e. x + y + z).
    double sum() const;

    //! Returns the average of all the components.
    double avg() const;

    //! Returns the minimum value among x, y, and z.
    double min() const;

    //! Returns the maximum value among x, y, and z.
    double max() const;


    //! Returns normalized vector.
    vector3 normalized() const;

    //! Returns the length of the vector.
    double length() const;

    //! Returns the squared length of the vector.
    double lengthSquared() const;

    //! Returns the distance to the other vector.
    double distanceTo(const vector3& other) const;

    //! Returns the squared distance to the other vector.
    double distanceSquaredTo(const vector3& other) const;

    //! Returns the reflection vector3 to the surface with given surface normal.
    vector3 reflected(const vector3& normal) const;

    //! Returns the projected vector3 to the surface with given surface normal.
    vector3 projected(const vector3& normal) const;

    //! Returns the tangential vector3 for this vector.
    void tangential( vector3& a, vector3& b ) const;

    //! Returns true if \p other is the same as this vector.
    bool isEqual(const vector3& other) const;

    bool isSimilar( const vector3& other, double eps = 0.000001f ) const;

    double& operator[](size_t i);
    const double& operator[](size_t i) const;

    //! Sedouble x and y with other vector3 \p pt.
    vector3& operator=(const vector3& v);

    //! Computes this += (v, v)
    vector3& operator+=(double v);

    //! Computes this += (v.x, v.y)
    vector3& operator+=(const vector3& v);

    //! Computes this -= (v, v)
    vector3& operator-=(double v);

    //! Computes this -= (v.x, v.y)
    vector3& operator-=(const vector3& v);

    //! Computes this *= (v, v)
    vector3& operator*=(double v);

    //! Computes this *= (v.x, v.y)
    vector3& operator*=(const vector3& v);

    //! Computes this /= (v, v)
    vector3& operator/=(double v);

    //! Computes this /= (v.x, v.y)
    vector3& operator/=(const vector3& v);

    //! Returns true if \p other is the same as this vector.
    bool operator==(const vector3& v) const;

    //! Returns true if \p other is the nodouble same as this vector.
    bool operator!=(const vector3& v) const;

   
};

vector3 operator+(const vector3& a);

vector3 operator-(const vector3& a);

vector3 operator+(const vector3& a, double b);

vector3 operator+(double a, const vector3& b);

vector3 operator+(const vector3& a, const vector3& b);

vector3 operator-(const vector3& a, double b);

vector3 operator-(double a, const vector3& b);

vector3 operator-(const vector3& a, const vector3& b);

vector3 operator*(const vector3& a, double b);

vector3 operator*( double a, const vector3& b);

vector3 operator*(const vector3& a, const vector3& b);

vector3 operator/(const vector3& a, double b);

vector3 operator/(double a, const vector3& b);

vector3 operator/(const vector3& a, const vector3& b);


