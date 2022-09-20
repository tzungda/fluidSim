//---fdmLinearSystem3.cpp

#include <cmath>
#include "fdmLinearSystem3.h"
#include "mathUtil.h"

void fdmBlas3::set(double s, dataBuffer3* result)
{
    result->set(s);
}

void fdmBlas3::set(const dataBuffer3& v, dataBuffer3* result) 
{
    result->set(v);
}

void fdmBlas3::set(double s, fdmMatrix3* result) 
{
    fdmMatrixRow3 row;
    row.center = row.right = row.up = row.front = s;
    result->set(row);
}

void fdmBlas3::set(const fdmMatrix3& m, fdmMatrix3* result) 
{
    result->set(m);
}

double fdmBlas3::dot(const dataBuffer3& a, const dataBuffer3& b) 
{
    size3 size = a.size();

    double result = 0.0;

    for (size_t k = 0; k < size.z; ++k) 
    {
        for (size_t j = 0; j < size.y; ++j) 
        {
            for (size_t i = 0; i < size.x; ++i) 
            {
                result += a(i, j, k) * b(i, j, k);
            }
        }
    }

    return result;
}

void fdmBlas3::axpy(
    double a,
    const dataBuffer3& x,
    const dataBuffer3& y,
    dataBuffer3* result) 
{
    size3 size = x.size();

    x.forEachIndex([&](size_t i, size_t j, size_t k) {
        (*result)(i, j, k) = a * x(i, j, k) + y(i, j, k);
        });
}

void fdmBlas3::mvm(
    const fdmMatrix3& m,
    const dataBuffer3& v,
    dataBuffer3* result) 
{
    size3 size = m.size();

    m.forEachIndex([&](size_t i, size_t j, size_t k) {
        (*result)(i, j, k)
            = m(i, j, k).center * v(i, j, k)
            + ((i > 0) ? m(i - 1, j, k).right * v(i - 1, j, k) : 0.0)
            + ((i + 1 < size.x) ? m(i, j, k).right * v(i + 1, j, k) : 0.0)
            + ((j > 0) ? m(i, j - 1, k).up * v(i, j - 1, k) : 0.0)
            + ((j + 1 < size.y) ? m(i, j, k).up * v(i, j + 1, k) : 0.0)
            + ((k > 0) ? m(i, j, k - 1).front * v(i, j, k - 1) : 0.0)
            + ((k + 1 < size.z) ? m(i, j, k).front * v(i, j, k + 1) : 0.0);
        });
}

void fdmBlas3::residual(
    const fdmMatrix3& a,
    const dataBuffer3& x,
    const dataBuffer3& b,
    dataBuffer3* result) 
{
    size3 size = a.size();

    a.forEachIndex([&](size_t i, size_t j, size_t k) {
        (*result)(i, j, k)
            = b(i, j, k)
            - a(i, j, k).center * x(i, j, k)
            - ((i > 0) ? a(i - 1, j, k).right * x(i - 1, j, k) : 0.0)
            - ((i + 1 < size.x) ? a(i, j, k).right * x(i + 1, j, k) : 0.0)
            - ((j > 0) ? a(i, j - 1, k).up * x(i, j - 1, k) : 0.0)
            - ((j + 1 < size.y) ? a(i, j, k).up * x(i, j + 1, k) : 0.0)
            - ((k > 0) ? a(i, j, k - 1).front * x(i, j, k - 1) : 0.0)
            - ((k + 1 < size.z) ? a(i, j, k).front * x(i, j, k + 1) : 0.0);
        });
}

double fdmBlas3::l2Norm(const dataBuffer3& v) 
{
    return std::sqrt(dot(v, v));
}

double fdmBlas3::lInfNorm(const dataBuffer3& v) 
{
    size3 size = v.size();

    double result = 0.0;

    for (size_t k = 0; k < size.z; ++k)
    {
        for (size_t j = 0; j < size.y; ++j) 
        {
            for (size_t i = 0; i < size.x; ++i) 
            {
                result = mathUtil::absmax(result, v(i, j, k));
            }
        }
    }

    return std::fabs(result);
}

