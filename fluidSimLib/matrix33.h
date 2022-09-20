//---matrix33.h
#ifndef matrix33_H
#define matrix33_H

#include <array>
#include "vector3.h"

class matrix33
{
public:
    matrix33();
    matrix33( double m00, double m01, double m02,
        double m10, double m11, double m12,
        double m20, double m21, double m22 );

    void set( double m00, double m01, double m02,
        double m10, double m11, double m12,
        double m20, double m21, double m22 );

    double trace() const;

    matrix33 add(const matrix33& mtx) const;

    vector3 mul(const vector3& v) const;
    matrix33 mul(const matrix33& mtx) const;

    matrix33 rmul(const matrix33& mtx) const;

    double& operator()(size_t i, size_t j);
    const double& operator()(size_t i, size_t j) const;

private:
    std::array<double, 9> m;
};

vector3 operator*(const matrix33& a, const vector3& b);

matrix33 operator*(const matrix33& a, const matrix33& b);

#endif


