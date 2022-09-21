//---fdmLinearSystem3.h
#ifndef fdmLinearSystem3_H
#define fdmLinearSystem3_H

#include "dataBuffer3.h"
#include "fdmMatrix3.h"

struct fdmLinearSystem3
{
    fdmMatrix3 A;
    dataBuffer3 x, b;
};

//! BLAS operator wrapper for 3-D finite differencing.
struct fdmBlas3 {
    typedef FloatType ScalarType;
    typedef dataBuffer3 VectorType;
    typedef fdmMatrix3 MatrixType;

    //! Sets entire element of given vector \p result with scalar \p s.
    static void set(FloatType s, dataBuffer3* result);
    
    //! Sets whole buffer to 0
    static void setZero( dataBuffer3* result );

    //! Copies entire element of given vector \p result with other vector \p v.
    static void set(const dataBuffer3& v, dataBuffer3* result);

    //! Sets entire element of given matrix \p result with scalar \p s.
    static void set(FloatType s, fdmMatrix3* result);

    //! Copies entire element of given matrix \p result with other matrix \p v.
    static void set(const fdmMatrix3& m, fdmMatrix3* result);

    //! Performs dot product with vector \p a and \p b.
    static FloatType dot(const dataBuffer3& a, const dataBuffer3& b);

    //! Performs ax + y operation where \p a is a matrix and \p x and \p y are
    //! vectors.
    static void axpy(
        FloatType a, const dataBuffer3& x, const dataBuffer3& y, dataBuffer3* result);

    //! Performs matrix-vector multiplication.
    static void mvm(
        const fdmMatrix3& m, const dataBuffer3& v, dataBuffer3* result);

    //! Computes residual vector (b - ax).
    static void residual(
        const fdmMatrix3& a,
        const dataBuffer3& x,
        const dataBuffer3& b,
        dataBuffer3* result);

    //! Returns L2-norm of the given vector \p v.
    static FloatType l2Norm(const dataBuffer3& v);

    //! Returns Linf-norm of the given vector \p v.
    static FloatType lInfNorm(const dataBuffer3& v);
};

#endif

