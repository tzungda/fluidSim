//---fdmIccgSolver3.cpp

#include <cmath>
#include <basetsd.h>
#include "fdmIccgSolver3.h"
#include "mathUtil.h"
#include "timer.h"

void fdmIccgSolver3::Preconditioner::build(const fdmMatrix3& matrix)
{
    size3 size = matrix.size();
    A = &matrix;

    d.resize(size, 0.0);
    y.resize(size, 0.0);

    matrix.forEachIndex([&](size_t i, size_t j, size_t k) {

        if ( matrix( i, j, k ).marker == 1 )
        {
            FloatType denom
                = matrix(i, j, k).center
                - ((i > 0) ?
                    mathUtil::square( matrix(i - 1, j, k).right ) * d(i - 1, j, k) : (FloatType)0.0)
                - ((j > 0) ?
                    mathUtil::square( matrix(i, j - 1, k).up )    * d(i, j - 1, k) : (FloatType)0.0)
                - ((k > 0) ?
                    mathUtil::square( matrix(i, j, k - 1).front ) * d(i, j, k - 1) : (FloatType)0.0);

            if (std::fabs(denom) > 0.0) {
                d(i, j, k) = (FloatType)1.0 / denom;
            } else {
                d(i, j, k) = 0.0;
            }
        }
        });
}


void fdmIccgSolver3::Preconditioner::solve(
    const dataBuffer3& b,
    dataBuffer3* x)
{
    size3 size = b.size();
    SSIZE_T sx = static_cast<SSIZE_T>(size.x);
    SSIZE_T sy = static_cast<SSIZE_T>(size.y);
    SSIZE_T sz = static_cast<SSIZE_T>(size.z);

   
    //--------------------------------------------------------------------------
    b.forEachIndex([&](size_t i, size_t j, size_t k)
    {
        if ((*A)(i, j, k).marker == 1 )
        {
            y(i, j, k)
                = (b(i, j, k)
                    - ((i > 0) ? (*A)(i - 1, j, k).right * y(i - 1, j, k) : (FloatType)0.0)
                    - ((j > 0) ? (*A)(i, j - 1, k).up    * y(i, j - 1, k) : (FloatType)0.0)
                    - ((k > 0) ? (*A)(i, j, k - 1).front * y(i, j, k - 1) : (FloatType)0.0))
                * d(i, j, k);
        }
        });
   
    //--------------------------------------------------------------------------
    //
    for (SSIZE_T k = sz - 1; k >= 0; --k)
    {
        for (SSIZE_T j = sy - 1; j >= 0; --j)
        {
            for (SSIZE_T i = sx - 1; i >= 0; --i)
            {
                if ((*A)(i, j, k).marker == 1 )
                {
                    (*x)(i, j, k)
                        = (y(i, j, k)
                            - ((i + 1 < sx) ?
                                (*A)(i, j, k).right * (*x)(i + 1, j, k) : (FloatType)0.0)
                            - ((j + 1 < sy) ?
                                (*A)(i, j, k).up    * (*x)(i, j + 1, k) : (FloatType)0.0)
                            - ((k + 1 < sz) ?
                                (*A)(i, j, k).front * (*x)(i, j, k + 1) : (FloatType)0.0))
                        * d(i, j, k);
                }
            }
        }
    }
       
}

//===========================================================================
void fdmIccgSolver3::DiagonalPreconditioner::build(const fdmMatrix3& matrix)
{
    size3 size = matrix.size();
    diag.resize( size, 0.0 );
    A = &matrix;

    length = size.x * size.y * size.z;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for ( SSIZE_T ind = 0; ind < length; ++ind )
    {
        if ( matrix.data()[ind].marker == 1 )
        {
            if ( abs( matrix.data()[ind].center ) > mathUtil::eps( ) )
            {
                diag.data()[ind] = matrix.data()[ind].center;
            }
            else
            {
                diag.data()[ind] = 1.0;
            }
        }
        else
        {
            diag.data()[ind] = 1.0;
        }
    }
   
}


void fdmIccgSolver3::DiagonalPreconditioner::solve(
    const dataBuffer3& b,
    dataBuffer3* x)
{
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for ( SSIZE_T ind = 0; ind < length; ++ind )
    {
        if ( A->data( )[ind].marker == 1 )
            x->data()[ind] = b.data()[ind]/diag.data()[ind];
    }
}

//===========================================================================
fdmIccgSolver3::fdmIccgSolver3(
    unsigned int maxNumberOfIterations,
    FloatType tolerance) :
    mMaxNumberOfIterations(maxNumberOfIterations),
    mLastNumberOfIterations(0),
    mTolerance(tolerance),
    mLastResidualNorm(std::numeric_limits<FloatType>::max())
{
}

bool fdmIccgSolver3::solve(fdmLinearSystem3* system)
{
    fdmMatrix3& matrix = system->A;
    dataBuffer3& solution = system->x;
    dataBuffer3& rhs = system->b;


    size3 size = matrix.size();
    mR.resize(size);
    mD.resize(size);
    mQ.resize(size);
    mS.resize(size);

    system->x.setZero();
    mR.setZero();
    mD.setZero();
    mQ.setZero();
    mS.setZero();

    {
        timer t("        mPrecond.build" );
        //mPrecond.build(matrix);
        mDiagPrecond.build( matrix );
    }

    {
        timer t("        fdmIccgSolver3::pcg" );
        fdmIccgSolver3::pcg(
            matrix,
            rhs,
            mMaxNumberOfIterations,
            mTolerance,
            &mDiagPrecond,
            //&mPrecond,
            &solution,
            &mR,
            &mD,
            &mQ,
            &mS,
            &mLastNumberOfIterations,
            &mLastResidualNorm);
    }

    printf( "        mLastNumberOfIterations = %d\n", (int)mLastNumberOfIterations );
    printf( "        mLastResidualNorm = %f\n", (float)mLastResidualNorm );

    return mLastResidualNorm <= mTolerance
        || mLastNumberOfIterations < mMaxNumberOfIterations;
}

unsigned int fdmIccgSolver3::maxNumberOfIterations() const
{
    return mMaxNumberOfIterations;
}

unsigned int fdmIccgSolver3::lastNumberOfIterations() const
{
    return mLastNumberOfIterations;
}

FloatType fdmIccgSolver3::tolerance() const
{
    return mTolerance;
}

FloatType fdmIccgSolver3::lastResidual() const
{
    return mLastResidualNorm;
}


void fdmIccgSolver3::pcg( const  fdmMatrix3& A,
    const dataBuffer3& b,
    unsigned int maxNumberOfIterations,
    FloatType tolerance,
    //Preconditioner* M,
    DiagonalPreconditioner *M,
    dataBuffer3* x,
    dataBuffer3* r,
    dataBuffer3* d,
    dataBuffer3* q,
    dataBuffer3* s,
    unsigned int* lastNumberOfIterations,
    FloatType* lastResidualNorm )
{
    fdmBlas3::setZero(r);
    fdmBlas3::setZero(d);
    fdmBlas3::setZero(q);
    fdmBlas3::setZero(s);

    {
        timer t("          M->build(A)" );
        M->build(A);
    }

    // r = b - Ax
    fdmBlas3::residual(A, *x, b, r);

    // d = M^-1r
    {
        timer t("          M->solve(*r, d)" );
        M->solve(*r, d);
    }

    // sigmaNew = r.d
    FloatType sigmaNew = fdmBlas3::dot(*r, *d);

    unsigned int iter = 0;
    bool trigger = false;
    while (sigmaNew > mathUtil::square(tolerance) && iter < maxNumberOfIterations) {
        // q = Ad
        fdmBlas3::mvm(A, *d, q);

        // alpha = sigmaNew/d.q
        FloatType alpha = sigmaNew / fdmBlas3::dot(*d, *q);

        // x = x + alpha*d
        fdmBlas3::axpy(alpha, *d, *x, x);

        // if i is divisible by 50...
        if (trigger || (iter % 50 == 0 && iter > 0))
        {
            // r = b - Ax
            fdmBlas3::residual(A, *x, b, r);
            trigger = false;
        }
        else
        {
            // r = r - alpha*q
            fdmBlas3::axpy(-alpha, *q, *r, r);
        }

        // s = M^-1r
        M->solve(*r, s);

        // sigmaOld = sigmaNew
        FloatType sigmaOld = sigmaNew;

        // sigmaNew = r.s
        sigmaNew = fdmBlas3::dot(*r, *s);

        if (sigmaNew > sigmaOld)
        {
            trigger = true;
        }

        // beta = sigmaNew/sigmaOld
        FloatType beta = sigmaNew / sigmaOld;

        // d = s + beta*d
        fdmBlas3::axpy(beta, *d, *s, d);

        ++iter;
    }

    *lastNumberOfIterations = iter;
    *lastResidualNorm = std::sqrt(sigmaNew);
}
