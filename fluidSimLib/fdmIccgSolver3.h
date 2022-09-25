//---fdmIccgSolver3.h
#ifndef fdmIccgSolver3_H
#define fdmIccgSolver3_H

#include "fdmLinearSystemSolver3.h"


class fdmIccgSolver3: public fdmLinearSystemSolver3
{
public:
    fdmIccgSolver3(unsigned int maxNumberOfIterations, FloatType tolerance);

    //! Solves the given linear system.
    bool solve( fdmLinearSystem3* system) override;

    //! Returns the max number of Jacobi iterations.
    unsigned int maxNumberOfIterations() const;

    //! Returns the last number of Jacobi iterations the solver made.
    unsigned int lastNumberOfIterations() const;

    //! Returns the max residual tolerance for the Jacobi method.
    FloatType tolerance() const;

    FloatType lastResidual() const;

    

private:
    struct Preconditioner final {
        const fdmMatrix3 *A;
        dataBuffer3 d;
        dataBuffer3 y;

        void build( const fdmMatrix3& matrix);

        void solve(
            const dataBuffer3& b,
            dataBuffer3* x);
    };

    unsigned int mMaxNumberOfIterations;
    unsigned int mLastNumberOfIterations;
    FloatType mTolerance;
    FloatType mLastResidualNorm;

    dataBuffer3 mR;
    dataBuffer3 mD;
    dataBuffer3 mQ;
    dataBuffer3 mS;
    Preconditioner mPrecond;

public:
    static void pcg( const  fdmMatrix3& A,
        const dataBuffer3& b,
        unsigned int maxNumberOfIterations,
        FloatType tolerance,
        Preconditioner* M,
        dataBuffer3* x,
        dataBuffer3* r,
        dataBuffer3* d,
        dataBuffer3* q,
        dataBuffer3* s,
        unsigned int* lastNumberOfIterations,
        FloatType* lastResidualNorm );
};

typedef std::shared_ptr<fdmIccgSolver3> fdmIccgSolver3Ptr;

#endif

