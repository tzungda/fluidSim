//---fdmLinearSystem3.cpp

#include <cmath>

#include "fdmLinearSystem3.h"
#include "mathUtil.h"

#ifdef SELFDEFINED_USE_TBB
#include <tbb/parallel_for.h>
#include <tbb/blocked_range3d.h>
#endif

void fdmBlas3::set(FloatType s, dataBuffer3* result)
{
    result->set(s);
}

void fdmBlas3::setZero( dataBuffer3* result )
{
    result->setZero();
}

void fdmBlas3::set(const dataBuffer3& v, dataBuffer3* result)
{
    result->set(v);
}

void fdmBlas3::set(FloatType s, fdmMatrix3* result)
{
    fdmMatrixRow3 row;
    row.center = row.right = row.up = row.front = s;
    result->set(row);
}

void fdmBlas3::set(const fdmMatrix3& m, fdmMatrix3* result)
{
    result->set(m);
}

FloatType fdmBlas3::dot(const dataBuffer3& a, const dataBuffer3& b)
{
    //size3 size = a.size();

    FloatType result = 0.0;
    size_t len = a.dataLength();
    const FloatType *ptrA = a.data();
    const FloatType *ptrB = b.data();
    for ( size_t i = 0; i < len; ++i )
    {
        result += (*ptrA++) * (*ptrB++);
    }

    /*for (size_t k = 0; k < size.z; ++k)
    {
        for (size_t j = 0; j < size.y; ++j)
        {
            for (size_t i = 0; i < size.x; ++i)
            {
                result += a(i, j, k) * b(i, j, k);
            }
        }
    }*/

    return result;
}

void fdmBlas3::axpy(
    FloatType a,
    const dataBuffer3& x,
    const dataBuffer3& y,
    dataBuffer3* result)
{
    size3 size = x.size();

#ifdef SELFDEFINED_USE_TBB
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, size.z, 0, size.y, 0, size.z),
        [&result, &a, &x, &y]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.cols().begin(), k_end=r.cols().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.pages().begin(), i_end=r.pages().end(); i<i_end; i++ )
                {
                    (*result)(i, j, k) = a * x(i, j, k) + y(i, j, k);
                }
            }
        }
    });

#elif _OPENMP
    x.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
        (*result)(i, j, k) = a * x(i, j, k) + y(i, j, k);
    });
#else
    x.forEachIndex([&](size_t i, size_t j, size_t k) {
        (*result)(i, j, k) = a * x(i, j, k) + y(i, j, k);
    });
#endif
    
}

void fdmBlas3::mvm(
    const fdmMatrix3& m,
    const dataBuffer3& v,
    dataBuffer3* result)
{
    size3 size = m.size();

#ifdef SELFDEFINED_USE_TBB
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, size.z, 0, size.y, 0, size.z),
        [&result, &m, &v, &size]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.cols().begin(), k_end=r.cols().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.pages().begin(), i_end=r.pages().end(); i<i_end; i++ )
                {
                    //printf("Hello World %d\n", i+j+k);
                    (*result)(i, j, k)
                        = m(i, j, k).center * v(i, j, k)
                        + ((i > 0) ? m(i - 1, j, k).right * v(i - 1, j, k) : (FloatType)0.0)
                        + ((i + 1 < size.x) ? m(i, j, k).right * v(i + 1, j, k) : (FloatType)0.0)
                        + ((j > 0) ? m(i, j - 1, k).up * v(i, j - 1, k) : (FloatType)0.0)
                        + ((j + 1 < size.y) ? m(i, j, k).up * v(i, j + 1, k) : (FloatType)0.0)
                        + ((k > 0) ? m(i, j, k - 1).front * v(i, j, k - 1) : (FloatType)0.0)
                        + ((k + 1 < size.z) ? m(i, j, k).front * v(i, j, k + 1) : (FloatType)0.0);
                }
            }
        }
    });
#else
#ifdef _OPENMP
    m.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
#else
    m.forEachIndex([&](size_t i, size_t j, size_t k) {
#endif
        (*result)(i, j, k)
            = m(i, j, k).center * v(i, j, k)
            + ((i > 0) ? m(i - 1, j, k).right * v(i - 1, j, k) : (FloatType)0.0)
            + ((i + 1 < size.x) ? m(i, j, k).right * v(i + 1, j, k) : (FloatType)0.0)
            + ((j > 0) ? m(i, j - 1, k).up * v(i, j - 1, k) : (FloatType)0.0)
            + ((j + 1 < size.y) ? m(i, j, k).up * v(i, j + 1, k) : (FloatType)0.0)
            + ((k > 0) ? m(i, j, k - 1).front * v(i, j, k - 1) : (FloatType)0.0)
            + ((k + 1 < size.z) ? m(i, j, k).front * v(i, j, k + 1) : (FloatType)0.0);
    });
#endif
    }

void fdmBlas3::residual(
    const fdmMatrix3& a,
    const dataBuffer3& x,
    const dataBuffer3& b,
    dataBuffer3* result)
{
    size3 size = a.size();

#ifdef SELFDEFINED_USE_TBB
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, size.z, 0, size.y, 0, size.z),
        [&result, &a, &x, &b, &size]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.cols().begin(), k_end=r.cols().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.pages().begin(), i_end=r.pages().end(); i<i_end; i++ )
                {
                    //printf("Hello World %d\n", i+j+k);
                    (*result)(i, j, k)
                        = b(i, j, k)
                        - a(i, j, k).center * x(i, j, k)
                        - ((i > 0) ? a(i - 1, j, k).right * x(i - 1, j, k) : (FloatType)0.0)
                        - ((i + 1 < size.x) ? a(i, j, k).right * x(i + 1, j, k) : (FloatType)0.0)
                        - ((j > 0) ? a(i, j - 1, k).up * x(i, j - 1, k) : (FloatType)0.0)
                        - ((j + 1 < size.y) ? a(i, j, k).up * x(i, j + 1, k) : (FloatType)0.0)
                        - ((k > 0) ? a(i, j, k - 1).front * x(i, j, k - 1) : (FloatType)0.0)
                        - ((k + 1 < size.z) ? a(i, j, k).front * x(i, j, k + 1) : (FloatType)0.0);
                }
            }
        }
    });

#else
#ifdef _OPENMP
    a.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
#else
    a.forEachIndex([&](size_t i, size_t j, size_t k) {
#endif
        (*result)(i, j, k)
            = b(i, j, k)
            - a(i, j, k).center * x(i, j, k)
            - ((i > 0) ? a(i - 1, j, k).right * x(i - 1, j, k) : (FloatType)0.0)
            - ((i + 1 < size.x) ? a(i, j, k).right * x(i + 1, j, k) : (FloatType)0.0)
            - ((j > 0) ? a(i, j - 1, k).up * x(i, j - 1, k) : (FloatType)0.0)
            - ((j + 1 < size.y) ? a(i, j, k).up * x(i, j + 1, k) : (FloatType)0.0)
            - ((k > 0) ? a(i, j, k - 1).front * x(i, j, k - 1) : (FloatType)0.0)
            - ((k + 1 < size.z) ? a(i, j, k).front * x(i, j, k + 1) : (FloatType)0.0);
    });
#endif
}

FloatType fdmBlas3::l2Norm(const dataBuffer3& v)
{
    return std::sqrt(dot(v, v));
}

FloatType fdmBlas3::lInfNorm(const dataBuffer3& v)
{
    size3 size = v.size();

    FloatType result = 0.0;

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

