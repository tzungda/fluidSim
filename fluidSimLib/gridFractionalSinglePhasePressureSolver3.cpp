//---gridFractionalSinglePhasePressureSolver3.cpp

#include "gridFractionalSinglePhasePressureSolver3.h"
#include "fdmIccgSolver3.h"
#include "gridFractionalBoundaryConditionSolver3.h"

#ifdef SELFDEFINED_USE_TBB
#include <tbb/parallel_for.h>
#include <tbb/blocked_range3d.h>
#endif

const FloatType kDefaultTolerance = (FloatType)1e-6;
const FloatType kMinWeight = (FloatType)0.01;

gridFractionalSinglePhasePressureSolver3::gridFractionalSinglePhasePressureSolver3()
{
    mSystemSolver = std::make_shared<fdmIccgSolver3>(100, kDefaultTolerance);
}

gridFractionalSinglePhasePressureSolver3::~gridFractionalSinglePhasePressureSolver3()
{}

void gridFractionalSinglePhasePressureSolver3::solve(
    const faceCenteredGrid3& input, FloatType timeIntervalInSeconds,
    faceCenteredGrid3 *output, const scalarField3& boundarySdf,
    const vectorField3& boundaryVelocity, const scalarField3& fluidSdf)
{
    buildWeights(input, boundarySdf, boundaryVelocity, fluidSdf);
    buildSystem(input);

    if (mSystemSolver != nullptr)
    {
        // solve the system
        mSystemSolver->solve(&mSystem);

        // apply pressure gradient
        applyPressureGradient(input, output);
    }
}

gridBoundaryConditionSolver3Ptr gridFractionalSinglePhasePressureSolver3::suggestedBoundaryConditionSolver() const
{
    return std::make_shared<gridFractionalBoundaryConditionSolver3>();
}

void gridFractionalSinglePhasePressureSolver3::setLinearSystemSolver( const fdmLinearSystemSolver3Ptr& solver )
{
    mSystemSolver = solver;
}

const dataBuffer3& gridFractionalSinglePhasePressureSolver3::pressure() const
{
    return mSystem.x;
}

void gridFractionalSinglePhasePressureSolver3::buildWeights(
    const faceCenteredGrid3& input, const scalarField3& boundarySdf,
    const vectorField3& boundaryVelocity, const scalarField3& fluidSdf)
{
    size3 uSize = input.uSize();
    size3 vSize = input.vSize();
    size3 wSize = input.wSize();
    auto uPos = input.uPosition();
    auto vPos = input.vPosition();
    auto wPos = input.wPosition();
    mWeightsU.resize(uSize);
    mWeightsV.resize(vSize);
    mWeightsW.resize(wSize);
    mBoundaryU.resize(uSize);
    mBoundaryV.resize(vSize);
    mBoundaryW.resize(wSize);
    mFluidSdf.resize(input.resolution(), input.gridSpacing(), input.origin());

    mFluidSdf.fill([&](const vector3& x) {
        return fluidSdf.sample(x);
    });

    vector3 h = input.gridSpacing();

#ifdef SELFDEFINED_USE_TBB
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, uSize.z, 0, uSize.y, 0, uSize.x),
        [this, &uPos, &boundarySdf, &boundaryVelocity, &h ]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.pages().begin(), k_end=r.pages().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.cols().begin(), i_end=r.cols().end(); i<i_end; i++ )
                {
                    vector3 pt = uPos(i, j, k);
                    FloatType phi0 =
                        boundarySdf.sample(pt + vector3(0.0, (FloatType)-0.5 * h.y, (FloatType)-0.5 * h.z));
                    FloatType phi1 =
                        boundarySdf.sample(pt + vector3(0.0, (FloatType)0.5 * h.y, (FloatType)-0.5 * h.z));
                    FloatType phi2 =
                        boundarySdf.sample(pt + vector3(0.0, (FloatType)-0.5 * h.y, (FloatType)0.5 * h.z));
                    FloatType phi3 =
                        boundarySdf.sample(pt + vector3(0.0, (FloatType)0.5 * h.y, (FloatType)0.5 * h.z));
                    FloatType frac = mathUtil::fractionInside(phi0, phi1, phi2, phi3);
                    FloatType weight = mathUtil::clamp((FloatType)1.0 - frac, 0.0, (FloatType)1.0);

                    // clamp non-zero weight to kMinWeight. Having nearly-zero element
                    // in the matrix can be an issue.
                    if (weight < kMinWeight && weight > 0.0)
                    {
                        weight = kMinWeight;
                    }

                    mWeightsU(i, j, k) = weight;
                    mBoundaryU(i, j, k) = boundaryVelocity.sample(pt).x;
                }
            }
        }
    });
#else
    mWeightsU.forEachIndex([&](size_t i, size_t j, size_t k)
    {
        vector3 pt = uPos(i, j, k);
        FloatType phi0 =
            boundarySdf.sample(pt + vector3(0.0, (FloatType)-0.5 * h.y, (FloatType)-0.5 * h.z));
        FloatType phi1 =
            boundarySdf.sample(pt + vector3(0.0, (FloatType)0.5 * h.y, (FloatType)-0.5 * h.z));
        FloatType phi2 =
            boundarySdf.sample(pt + vector3(0.0, (FloatType)-0.5 * h.y, (FloatType)0.5 * h.z));
        FloatType phi3 =
            boundarySdf.sample(pt + vector3(0.0, (FloatType)0.5 * h.y, (FloatType)0.5 * h.z));
        FloatType frac = mathUtil::fractionInside(phi0, phi1, phi2, phi3);
        FloatType weight = mathUtil::clamp((FloatType)1.0 - frac, 0.0, (FloatType)1.0);

        // clamp non-zero weight to kMinWeight. Having nearly-zero element
        // in the matrix can be an issue.
        if (weight < kMinWeight && weight > 0.0)
        {
            weight = kMinWeight;
        }

        mWeightsU(i, j, k) = weight;
        mBoundaryU(i, j, k) = boundaryVelocity.sample(pt).x;
    });
#endif

#ifdef SELFDEFINED_USE_TBB
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, vSize.z, 0, vSize.y, 0, vSize.x),
        [this, &vPos, &boundarySdf, &boundaryVelocity, &h ]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.pages().begin(), k_end=r.pages().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.cols().begin(), i_end=r.cols().end(); i<i_end; i++ )
                {
                    vector3 pt = vPos(i, j, k);
                    FloatType phi0 =
                        boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, 0.0, (FloatType)-0.5 * h.z));
                    FloatType phi1 =
                        boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, 0.0, (FloatType)0.5 * h.z));
                    FloatType phi2 =
                        boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, 0.0, (FloatType)-0.5 * h.z));
                    FloatType phi3 =
                        boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, 0.0, (FloatType)0.5 * h.z));
                    FloatType frac = mathUtil::fractionInside(phi0, phi1, phi2, phi3);
                    FloatType weight = mathUtil::clamp((FloatType)1.0 - frac, 0.0, (FloatType)1.0);

                    // clamp non-zero weight to kMinWeight. Having nearly-zero element
                    // in the matrix can be an issue.
                    if (weight < kMinWeight && weight > 0.0)
                    {
                        weight = kMinWeight;
                    }

                    mWeightsV(i, j, k) = weight;
                    mBoundaryV(i, j, k) = boundaryVelocity.sample(pt).y;
                }
            }
        }
    });
#else
    mWeightsV.forEachIndex([&](size_t i, size_t j, size_t k)
    {
        vector3 pt = vPos(i, j, k);
        FloatType phi0 =
            boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, 0.0, (FloatType)-0.5 * h.z));
        FloatType phi1 =
            boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, 0.0, (FloatType)0.5 * h.z));
        FloatType phi2 =
            boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, 0.0, (FloatType)-0.5 * h.z));
        FloatType phi3 =
            boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, 0.0, (FloatType)0.5 * h.z));
        FloatType frac = mathUtil::fractionInside(phi0, phi1, phi2, phi3);
        FloatType weight = mathUtil::clamp((FloatType)1.0 - frac, 0.0, (FloatType)1.0);

        // clamp non-zero weight to kMinWeight. Having nearly-zero element
        // in the matrix can be an issue.
        if (weight < kMinWeight && weight > 0.0)
        {
            weight = kMinWeight;
        }

        mWeightsV(i, j, k) = weight;
        mBoundaryV(i, j, k) = boundaryVelocity.sample(pt).y;
    });
#endif

#ifdef SELFDEFINED_USE_TBB
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, wSize.z, 0, wSize.y, 0, wSize.x),
        [this, &wPos, &boundarySdf, &boundaryVelocity, &h ]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.pages().begin(), k_end=r.pages().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.cols().begin(), i_end=r.cols().end(); i<i_end; i++ )
                {
                    vector3 pt = wPos(i, j, k);
                    FloatType phi0 =
                        boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, (FloatType)-0.5 * h.y, 0.0));
                    FloatType phi1 =
                        boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, (FloatType)0.5 * h.y, 0.0));
                    FloatType phi2 =
                        boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, (FloatType)-0.5 * h.y, 0.0));
                    FloatType phi3 =
                        boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, (FloatType)0.5 * h.y, 0.0));
                    FloatType frac = mathUtil::fractionInside(phi0, phi1, phi2, phi3);
                    FloatType weight = mathUtil::clamp((FloatType)1.0 - frac, 0.0, (FloatType)1.0);

                    // clamp non-zero weight to kMinWeight. Having nearly-zero element
                    // in the matrix can be an issue.
                    if (weight < kMinWeight && weight > 0.0) {
                        weight = kMinWeight;
                    }

                    mWeightsW(i, j, k) = weight;
                    mBoundaryW(i, j, k) = boundaryVelocity.sample(pt).z;
                }
            }
        }
    });
#else
    mWeightsW.forEachIndex([&](size_t i, size_t j, size_t k)
    {
        vector3 pt = wPos(i, j, k);
        FloatType phi0 =
            boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, (FloatType)-0.5 * h.y, 0.0));
        FloatType phi1 =
            boundarySdf.sample(pt + vector3((FloatType)-0.5 * h.x, (FloatType)0.5 * h.y, 0.0));
        FloatType phi2 =
            boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, (FloatType)-0.5 * h.y, 0.0));
        FloatType phi3 =
            boundarySdf.sample(pt + vector3((FloatType)0.5 * h.x, (FloatType)0.5 * h.y, 0.0));
        FloatType frac = mathUtil::fractionInside(phi0, phi1, phi2, phi3);
        FloatType weight = mathUtil::clamp((FloatType)1.0 - frac, 0.0, (FloatType)1.0);

        // clamp non-zero weight to kMinWeight. Having nearly-zero element
        // in the matrix can be an issue.
        if (weight < kMinWeight && weight > 0.0) {
            weight = kMinWeight;
        }

        mWeightsW(i, j, k) = weight;
        mBoundaryW(i, j, k) = boundaryVelocity.sample(pt).z;
    });
#endif
}

void gridFractionalSinglePhasePressureSolver3::buildSystem(
    const faceCenteredGrid3& input)
{
    size3 size = input.resolution();
    mSystem.A.resize(size);
    mSystem.x.resize(size);
    mSystem.b.resize(size);

    vector3 invH = 1.0 / input.gridSpacing();
    vector3 invHSqr = invH * invH;

    // Build linear system
    mSystem.A.forEachIndex([&](size_t i, size_t j, size_t k) {
        auto& row = mSystem.A(i, j, k);

        // initialize
        row.center = row.right = row.up = row.front = 0.0;
        mSystem.b(i, j, k) = 0.0;

        FloatType centerPhi = mFluidSdf(i, j, k);

        if ( mathUtil::isInsideSdf(centerPhi))
        {
            row.marker = 1;

            FloatType term = 0.0;

            if (i + 1 < size.x)
            {
                term = mWeightsU(i + 1, j, k) * invHSqr.x;
                FloatType rightPhi = mFluidSdf(i + 1, j, k);
                if ( mathUtil::isInsideSdf(rightPhi)) {
                    row.center += term;
                    row.right -= term;
                }
                else
                {
                    FloatType theta = mathUtil::fractionInsideSdf(centerPhi, rightPhi);
                    theta = std::max<FloatType>(theta, (FloatType)0.01);
                    row.center += term / theta;
                }
                mSystem.b(i, j, k) +=
                    mWeightsU(i + 1, j, k) * input.u(i + 1, j, k) * invH.x;
            }
            else
            {
                mSystem.b(i, j, k) += input.u(i + 1, j, k) * invH.x;
            }

            if (i > 0)
            {
                term = mWeightsU(i, j, k) * invHSqr.x;
                FloatType leftPhi = mFluidSdf(i - 1, j, k);
                if (mathUtil::isInsideSdf(leftPhi))
                {
                    row.center += term;
                }
                else
                {
                    FloatType theta = mathUtil::fractionInsideSdf(centerPhi, leftPhi);
                    theta = std::max<FloatType>(theta, (FloatType)0.01);
                    row.center += term / theta;
                }
                mSystem.b(i, j, k) -=
                    mWeightsU(i, j, k) * input.u(i, j, k) * invH.x;
            }
            else
            {
                mSystem.b(i, j, k) -= input.u(i, j, k) * invH.x;
            }

            if (j + 1 < size.y)
            {
                term = mWeightsV(i, j + 1, k) * invHSqr.y;
                FloatType upPhi = mFluidSdf(i, j + 1, k);
                if (mathUtil::isInsideSdf(upPhi))
                {
                    row.center += term;
                    row.up -= term;
                }
                else
                {
                    FloatType theta = mathUtil::fractionInsideSdf(centerPhi, upPhi);
                    theta = std::max<FloatType>(theta, (FloatType)0.01);
                    row.center += term / theta;
                }
                mSystem.b(i, j, k) +=
                    mWeightsV(i, j + 1, k) * input.v(i, j + 1, k) * invH.y;
            }
            else
            {
                mSystem.b(i, j, k) += input.v(i, j + 1, k) * invH.y;
            }

            if (j > 0)
            {
                term = mWeightsV(i, j, k) * invHSqr.y;
                FloatType downPhi = mFluidSdf(i, j - 1, k);
                if (mathUtil::isInsideSdf(downPhi))
                {
                    row.center += term;
                }
                else
                {
                    FloatType theta = mathUtil::fractionInsideSdf(centerPhi, downPhi);
                    theta = std::max<FloatType>(theta, (FloatType)0.01);
                    row.center += term / theta;
                }
                mSystem.b(i, j, k) -=
                    mWeightsV(i, j, k) * input.v(i, j, k) * invH.y;
            }
            else
            {
                mSystem.b(i, j, k) -= input.v(i, j, k) * invH.y;
            }

            if (k + 1 < size.z)
            {
                term = mWeightsW(i, j, k + 1) * invHSqr.z;
                FloatType frontPhi = mFluidSdf(i, j, k + 1);
                if (mathUtil::isInsideSdf(frontPhi))
                {
                    row.center += term;
                    row.front -= term;
                }
                else
                {
                    FloatType theta = mathUtil::fractionInsideSdf(centerPhi, frontPhi);
                    theta = std::max<FloatType>(theta, (FloatType)0.01);
                    row.center += term / theta;
                }
                mSystem.b(i, j, k) +=
                    mWeightsW(i, j, k + 1) * input.w(i, j, k + 1) * invH.z;
            }
            else
            {
                mSystem.b(i, j, k) += input.w(i, j, k + 1) * invH.z;
            }

            if (k > 0)
            {
                term = mWeightsW(i, j, k) * invHSqr.z;
                FloatType backPhi = mFluidSdf(i, j, k - 1);
                if (mathUtil::isInsideSdf(backPhi))
                {
                    row.center += term;
                }
                else
                {
                    FloatType theta = mathUtil::fractionInsideSdf(centerPhi, backPhi);
                    theta = std::max<FloatType>(theta, (FloatType)0.01);
                    row.center += term / theta;
                }
                mSystem.b(i, j, k) -=
                    mWeightsW(i, j, k) * input.w(i, j, k) * invH.z;
            }
            else
            {
                mSystem.b(i, j, k) -= input.w(i, j, k) * invH.z;
            }

            // Accumulate contributions from the moving boundary
            FloatType boundaryContribution
                = ((FloatType)1.0 - mWeightsU(i + 1, j, k)) * mBoundaryU(i + 1, j, k) * invH.x
                - ((FloatType)1.0 - mWeightsU(i, j, k)) * mBoundaryU(i, j, k) * invH.x
                + ((FloatType)1.0 - mWeightsV(i, j + 1, k)) * mBoundaryV(i, j + 1, k) * invH.y
                - ((FloatType)1.0 - mWeightsV(i, j, k)) * mBoundaryV(i, j, k) * invH.y
                + ((FloatType)1.0 - mWeightsW(i, j, k + 1)) * mBoundaryW(i, j, k + 1) * invH.z
                - ((FloatType)1.0 - mWeightsW(i, j, k)) * mBoundaryW(i, j, k) * invH.z;
            mSystem.b(i, j, k) += boundaryContribution;
        } else {
            row.center = 1.0;
        }
    });
}

void gridFractionalSinglePhasePressureSolver3::applyPressureGradient(
    const faceCenteredGrid3& input, faceCenteredGrid3 *output)
{
    size3 size = input.resolution();

    vector3 invH = 1.0 / input.gridSpacing();

#ifdef SELFDEFINED_USE_TBB
    size3 s = mSystem.x.size();
    tbb::parallel_for( tbb::blocked_range3d<size_t>(0, s.z, 0, s.y, 0, s.x),
        [this, &size, output, &input, &invH ]( const tbb::blocked_range3d<size_t> &r ) {
        for( size_t k=r.pages().begin(), k_end=r.pages().end(); k<k_end; k++)
        {
            for( size_t j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++)
            {
                for( size_t i=r.cols().begin(), i_end=r.cols().end(); i<i_end; i++ )
                {
                    FloatType centerPhi = mFluidSdf(i, j, k);

                    if (i + 1 < size.x && mWeightsU(i + 1, j, k) > 0.0 &&
                        (mathUtil::isInsideSdf(centerPhi) || mathUtil::isInsideSdf(mFluidSdf(i + 1, j, k))))
                    {
                        FloatType rightPhi = mFluidSdf(i + 1, j, k);
                        FloatType theta = mathUtil::fractionInsideSdf(centerPhi, rightPhi);
                        theta = std::max<FloatType>(theta, (FloatType)0.01);
                        //
                        output->u( i + 1, j, k ) = input.u( i + 1, j, k ) +
                            invH.x / theta * (mSystem.x(i + 1, j, k) - mSystem.x(i, j, k));
                    }

                    if (j + 1 < size.y && mWeightsV(i, j + 1, k) > 0.0 &&
                        (mathUtil::isInsideSdf(centerPhi) || mathUtil::isInsideSdf(mFluidSdf(i, j + 1, k))))
                    {
                        FloatType upPhi = mFluidSdf(i, j + 1, k);
                        FloatType theta = mathUtil::fractionInsideSdf(centerPhi, upPhi);
                        theta = std::max<FloatType>(theta, (FloatType)0.01);
                        //
                        output->v( i, j + 1, k ) = input.v( i, j + 1, k ) +
                            invH.y / theta * (mSystem.x(i, j + 1, k) - mSystem.x(i, j, k));
                    }

                    if (k + 1 < size.z && mWeightsW(i, j, k + 1) > 0.0 &&
                        (mathUtil::isInsideSdf(centerPhi) || mathUtil::isInsideSdf(mFluidSdf(i, j, k + 1))))
                    {
                        FloatType frontPhi = mFluidSdf(i, j, k + 1);
                        FloatType theta = mathUtil::fractionInsideSdf(centerPhi, frontPhi);
                        theta = std::max<FloatType>(theta, (FloatType)0.01);
                        //
                        output->w( i, j, k + 1 ) = input.w( i, j, k + 1 ) +
                            invH.z / theta * (mSystem.x(i, j, k + 1) - mSystem.x(i, j, k));
                    }
                }
            }
        }
    });
#else
    mSystem.x.forEachIndex([&](size_t i, size_t j, size_t k) 
    {
        FloatType centerPhi = mFluidSdf(i, j, k);

        if (i + 1 < size.x && mWeightsU(i + 1, j, k) > 0.0 &&
            (mathUtil::isInsideSdf(centerPhi) || mathUtil::isInsideSdf(mFluidSdf(i + 1, j, k))))
        {
            FloatType rightPhi = mFluidSdf(i + 1, j, k);
            FloatType theta = mathUtil::fractionInsideSdf(centerPhi, rightPhi);
            theta = std::max<FloatType>(theta, (FloatType)0.01);
            //
            output->u( i + 1, j, k ) = input.u( i + 1, j, k ) +
                invH.x / theta * (mSystem.x(i + 1, j, k) - mSystem.x(i, j, k));
        }

        if (j + 1 < size.y && mWeightsV(i, j + 1, k) > 0.0 &&
            (mathUtil::isInsideSdf(centerPhi) || mathUtil::isInsideSdf(mFluidSdf(i, j + 1, k))))
        {
            FloatType upPhi = mFluidSdf(i, j + 1, k);
            FloatType theta = mathUtil::fractionInsideSdf(centerPhi, upPhi);
            theta = std::max<FloatType>(theta, (FloatType)0.01);
            //
            output->v( i, j + 1, k ) = input.v( i, j + 1, k ) +
                invH.y / theta * (mSystem.x(i, j + 1, k) - mSystem.x(i, j, k));
        }

        if (k + 1 < size.z && mWeightsW(i, j, k + 1) > 0.0 &&
            (mathUtil::isInsideSdf(centerPhi) || mathUtil::isInsideSdf(mFluidSdf(i, j, k + 1))))
        {
            FloatType frontPhi = mFluidSdf(i, j, k + 1);
            FloatType theta = mathUtil::fractionInsideSdf(centerPhi, frontPhi);
            theta = std::max<FloatType>(theta, (FloatType)0.01);
            //
            output->w( i, j, k + 1 ) = input.w( i, j, k + 1 ) +
                invH.z / theta * (mSystem.x(i, j, k + 1) - mSystem.x(i, j, k));
        }
    });
#endif
}


