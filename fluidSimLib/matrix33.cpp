//---matrix33.cpp

#include "matrix33.h"

matrix33::matrix33()
{
    m[0] = 1.0, m[1] = 0.0, m[2] = 0.0;
    m[3] = 0.0, m[4] = 1.0, m[5] = 0.0;
    m[6] = 0.0, m[7] = 0.0, m[8] = 1.0;
}

matrix33::matrix33( FloatType m00, FloatType m01, FloatType m02,
    FloatType m10, FloatType m11, FloatType m12,
    FloatType m20, FloatType m21, FloatType m22 )
{
    set( m00, m01, m02,
         m10, m11, m12,
         m20, m21, m22 );
}

void matrix33::set( FloatType m00, FloatType m01, FloatType m02,
    FloatType m10, FloatType m11, FloatType m12,
    FloatType m20, FloatType m21, FloatType m22 )
{
    m[0] = m00;
    m[1] = m01;
    m[2] = m02;
    m[3] = m10;
    m[4] = m11;
    m[5] = m12;
    m[6] = m20;
    m[7] = m21;
    m[8] = m22;
}

FloatType matrix33::trace() const
{
    return m[0] + m[4] + m[8];
}

matrix33 matrix33::add(const matrix33& mtx) const
{
    return matrix33(
        m[0] + mtx.m[0],
        m[1] + mtx.m[1],
        m[2] + mtx.m[2],
        m[3] + mtx.m[3],
        m[4] + mtx.m[4],
        m[5] + mtx.m[5],
        m[6] + mtx.m[6],
        m[7] + mtx.m[7],
        m[8] + mtx.m[8]);
}

vector3 matrix33::mul(const vector3& v) const
{
    return vector3(
        m[0] * v.x + m[1] * v.y + m[2] * v.z,
        m[3] * v.x + m[4] * v.y + m[5] * v.z,
        m[6] * v.x + m[7] * v.y + m[8] * v.z);
}

matrix33 matrix33::mul(const matrix33& mtx) const
{
    return matrix33(
        m[0] * mtx.m[0]
        + m[1] * mtx.m[3]
        + m[2] * mtx.m[6],
        m[0] * mtx.m[1]
        + m[1] * mtx.m[4]
        + m[2] * mtx.m[7],
        m[0] * mtx.m[2]
        + m[1] * mtx.m[5]
        + m[2] * mtx.m[8],

        m[3] * mtx.m[0]
        + m[4] * mtx.m[3]
        + m[5] * mtx.m[6],
        m[3] * mtx.m[1]
        + m[4] * mtx.m[4]
        + m[5] * mtx.m[7],
        m[3] * mtx.m[2]
        + m[4] * mtx.m[5]
        + m[5] * mtx.m[8],

        m[6] * mtx.m[0]
        + m[7] * mtx.m[3]
        + m[8] * mtx.m[6],
        m[6] * mtx.m[1]
        + m[7] * mtx.m[4]
        + m[8] * mtx.m[7],
        m[6] * mtx.m[2]
        + m[7] * mtx.m[5]
        + m[8] * mtx.m[8]);
}

matrix33 matrix33::rmul(const matrix33& mtx ) const
{
    return mtx.mul(*this);
}

FloatType& matrix33::operator()(SizeType i, SizeType j)
{
    return m[3 * i + j];
}

const FloatType& matrix33::operator()(SizeType i, SizeType j) const
{
    return m[3 * i + j];
}


vector3 operator*(const matrix33& a, const vector3& b)
{
    return a.mul(b);
}

matrix33 operator*(const matrix33& a, const matrix33& b)
{
    return a.mul(b);
}


