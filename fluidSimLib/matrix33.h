//---matrix33.h
#ifndef matrix33_H
#define matrix33_H

#include <array>
#include "vector3.h"

class matrix33
{
public:
    matrix33();
    matrix33( FloatType m00, FloatType m01, FloatType m02,
        FloatType m10, FloatType m11, FloatType m12,
        FloatType m20, FloatType m21, FloatType m22 );

    void set( FloatType m00, FloatType m01, FloatType m02,
        FloatType m10, FloatType m11, FloatType m12,
        FloatType m20, FloatType m21, FloatType m22 );

    FloatType trace() const;

    matrix33 add(const matrix33& mtx) const;

    vector3 mul(const vector3& v) const;
    matrix33 mul(const matrix33& mtx) const;

    matrix33 rmul(const matrix33& mtx) const;

    FloatType& operator()(SizeType i, SizeType j);
    const FloatType& operator()(SizeType i, SizeType j) const;

private:
    std::array<FloatType, 9> m;
};

vector3 operator*(const matrix33& a, const vector3& b);

matrix33 operator*(const matrix33& a, const matrix33& b);

#endif


