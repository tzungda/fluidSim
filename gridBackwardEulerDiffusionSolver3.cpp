//---gridBackwardEulerDiffusionSolver3.cpp

#include "gridBackwardEulerDiffusionSolver3.h"
#include "fdmIccgSolver3.h"

const char kFluid = 0;
const char kAir = 1;
const char kBoundary = 2;

gridBackwardEulerDiffusionSolver3::gridBackwardEulerDiffusionSolver3(
    boundaryType bndType) : mBoundaryType(bndType)
{
    mSystemSolver = std::make_shared<fdmIccgSolver3>(100, mathUtil::eps() );
}

void gridBackwardEulerDiffusionSolver3::solve(
    const scalarGrid3& source,
    FloatType diffusionCoefficient,
    FloatType timeIntervalInSeconds,
    scalarGrid3* dest,
    const scalarField3& boundarySdf,
    const scalarField3& fluidSdf) 
{
    auto pos = source.dataPosition();
    vector3 h = source.gridSpacing();
    vector3 c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

    buildMarkers(source.dataSize(), pos, boundarySdf, fluidSdf);
    buildMatrix(source.dataSize(), c);
    buildVectors(source.data(), c);

    if (mSystemSolver != nullptr) 
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachDataPointIndex(
            [&](size_t i, size_t j, size_t k) {
                (*dest)(i, j, k) = mSystem.x(i, j, k);
            });
    }
}

void gridBackwardEulerDiffusionSolver3::solve(
    const extendVectorGrid3& source,
    FloatType diffusionCoefficient,
    FloatType timeIntervalInSeconds,
    extendVectorGrid3* dest,
    const scalarField3& boundarySdf,
    const scalarField3& fluidSdf)
{
    auto pos = source.dataPosition();
    vector3 h = source.gridSpacing();
    vector3 c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

    buildMarkers(source.dataSize(), pos, boundarySdf, fluidSdf);
    buildMatrix(source.dataSize(), c);

    // u
    buildVectors(source.data(), c, 0);

    if (mSystemSolver != nullptr) 
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachDataPointIndex(
            [&](size_t i, size_t j, size_t k) {
                (*dest)(i, j, k).x = mSystem.x(i, j, k);
            });
    }

    // v
    buildVectors(source.data(), c, 1);

    if (mSystemSolver != nullptr) 
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachDataPointIndex(
            [&](size_t i, size_t j, size_t k) {
                (*dest)(i, j, k).y = mSystem.x(i, j, k);
            });
    }

    // w
    buildVectors(source.data(), c, 2);

    if (mSystemSolver != nullptr)
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachDataPointIndex(
            [&](size_t i, size_t j, size_t k) {
                (*dest)(i, j, k).z = mSystem.x(i, j, k);
            });
    }
}

void gridBackwardEulerDiffusionSolver3::solve(
    const faceCenteredGrid3& source,
    FloatType diffusionCoefficient,
    FloatType timeIntervalInSeconds,
    faceCenteredGrid3* dest,
    const scalarField3& boundarySdf,
    const scalarField3& fluidSdf)
{
    vector3 h = source.gridSpacing();
    vector3 c = timeIntervalInSeconds * diffusionCoefficient / (h * h);

    // u
    auto uPos = source.uPosition();
    buildMarkers(source.uSize(), uPos, boundarySdf, fluidSdf);
    buildMatrix(source.uSize(), c);
    buildVectors(source.uData(), c);

    if (mSystemSolver != nullptr)
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachUIndex(
            [&](size_t i, size_t j, size_t k) {
                dest->u(i, j, k) = mSystem.x(i, j, k);
            });
    }

    // v
    auto vPos = source.vPosition();
    buildMarkers(source.vSize(), vPos, boundarySdf, fluidSdf);
    buildMatrix(source.vSize(), c);
    buildVectors(source.vData(), c);

    if (mSystemSolver != nullptr)
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachVIndex(
            [&](size_t i, size_t j, size_t k) {
                dest->v(i, j, k) = mSystem.x(i, j, k);
            });
    }

    // w
    auto wPos = source.wPosition();
    buildMarkers(source.wSize(), wPos, boundarySdf, fluidSdf);
    buildMatrix(source.wSize(), c);
    buildVectors(source.wData(), c);

    if (mSystemSolver != nullptr) 
    {
        // Solve the system
        mSystemSolver->solve(&mSystem);

        // Assign the solution
        source.forEachWIndex(
            [&](size_t i, size_t j, size_t k) {
                dest->w(i, j, k) = mSystem.x(i, j, k);
            });
    }
}

void gridBackwardEulerDiffusionSolver3::setLinearSystemSolver(
    const fdmLinearSystemSolver3Ptr& solver) 
{
    mSystemSolver = solver;
}

void gridBackwardEulerDiffusionSolver3::buildMarkers(
    const size3& size,
    const std::function<vector3(size_t, size_t, size_t)>& pos,
    const scalarField3& boundarySdf,
    const scalarField3& fluidSdf) 
{
    mMarkers.resize(size);

#ifdef _OPENMP
    mMarkers.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
#else
    mMarkers.forEachIndex([&](size_t i, size_t j, size_t k) {
#endif
        if (mathUtil::isInsideSdf(boundarySdf.sample(pos(i, j, k)))) {
            mMarkers(i, j, k) = kBoundary;
        } else if (mathUtil::isInsideSdf(fluidSdf.sample(pos(i, j, k)))) {
            mMarkers(i, j, k) = kFluid;
        } else {
            mMarkers(i, j, k) = kAir;
        }
        });
}

void gridBackwardEulerDiffusionSolver3::buildMatrix(
    const size3& size,
    const vector3& c) 
{
    mSystem.A.resize(size);

    bool isDirichlet = (mBoundaryType == Dirichlet);

    // Build linear system
    mSystem.A.forEachIndex(
        [&](size_t i, size_t j, size_t k) {
            auto& row = mSystem.A(i, j, k);

            // Initialize
            row.center = 1.0;
            row.right = row.up = row.front = 0.0;

            if (mMarkers(i, j, k) == kFluid) 
            {
                if (i + 1 < size.x) {
                    if ((isDirichlet && mMarkers(i + 1, j, k) != kAir)
                        || mMarkers(i + 1, j, k) == kFluid) 
                    {
                        row.center += c.x;
                    }

                    if (mMarkers(i + 1, j, k) == kFluid) 
                    {
                        row.right -=  c.x;
                    }
                }

                if (i > 0
                    && ((isDirichlet && mMarkers(i - 1, j, k) != kAir)
                        || mMarkers(i - 1, j, k) == kFluid)) 
                {
                    row.center += c.x;
                }

                if (j + 1 < size.y) {
                    if ((isDirichlet && mMarkers(i, j + 1, k) != kAir)
                        || mMarkers(i, j + 1, k) == kFluid) 
                    {
                        row.center += c.y;
                    }

                    if (mMarkers(i, j + 1, k) == kFluid) 
                    {
                        row.up -=  c.y;
                    }
                }

                if (j > 0
                    && ((isDirichlet && mMarkers(i, j - 1, k) != kAir)
                        || mMarkers(i, j - 1, k) == kFluid)) 
                {
                    row.center += c.y;
                }

                if (k + 1 < size.z) 
                {
                    if ((isDirichlet && mMarkers(i, j, k + 1) != kAir)
                        || mMarkers(i, j, k + 1) == kFluid) 
                    {
                        row.center += c.z;
                    }

                    if (mMarkers(i, j, k + 1) == kFluid) 
                    {
                        row.front -=  c.z;
                    }
                }

                if (k > 0
                    && ((isDirichlet && mMarkers(i, j, k - 1) != kAir)
                        || mMarkers(i, j, k - 1) == kFluid))
                {
                    row.center += c.z;
                }
            }
        });
}

void gridBackwardEulerDiffusionSolver3::buildVectors(
    const dataBuffer3& f,
    const vector3& c) 
{
    size3 size = f.size();

    mSystem.x.resize(size, 0.0);
    mSystem.b.resize(size, 0.0);

    // Build linear system
    mSystem.x.forEachIndex(
        [&](size_t i, size_t j, size_t k) 
        {
            mSystem.b(i, j, k) = mSystem.x(i, j, k) = f(i, j, k);

            if (mBoundaryType == Dirichlet && mMarkers(i, j, k) == kFluid) {
                if (i + 1 < size.x && mMarkers(i + 1, j, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.x * f(i + 1, j, k);
                }

                if (i > 0 && mMarkers(i - 1, j, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.x * f(i - 1, j, k);
                }

                if (j + 1 < size.y && mMarkers(i, j + 1, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.y * f(i, j + 1, k);
                }

                if (j > 0 && mMarkers(i, j - 1, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.y * f(i, j - 1, k);
                }

                if (k + 1 < size.z && mMarkers(i, j, k + 1) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.z * f(i, j, k + 1);
                }

                if (k > 0 && mMarkers(i, j, k - 1) == kBoundary)
                {
                    mSystem.b(i, j, k) += c.z * f(i, j, k - 1);
                }
            }
        });
}

void gridBackwardEulerDiffusionSolver3::buildVectors(
    const vecDataBuffer3& f,
    const vector3& c,
    size_t component)
{
    size3 size = f.size();

    mSystem.x.resize(size, 0.0);
    mSystem.b.resize(size, 0.0);

    // Build linear system
    mSystem.x.forEachIndex(
        [&](size_t i, size_t j, size_t k) {
            mSystem.b(i, j, k) = mSystem.x(i, j, k) = f(i, j, k)[component];

            if (mBoundaryType == Dirichlet && mMarkers(i, j, k) == kFluid) 
            {
                if (i + 1 < size.x && mMarkers(i + 1, j, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.x * f(i + 1, j, k)[component];
                }

                if (i > 0 && mMarkers(i - 1, j, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.x * f(i - 1, j, k)[component];
                }

                if (j + 1 < size.y && mMarkers(i, j + 1, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.y * f(i, j + 1, k)[component];
                }

                if (j > 0 && mMarkers(i, j - 1, k) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.y * f(i, j - 1, k)[component];
                }

                if (k + 1 < size.z && mMarkers(i, j, k + 1) == kBoundary)
                {
                    mSystem.b(i, j, k) += c.z * f(i, j, k + 1)[component];
                }

                if (k > 0 && mMarkers(i, j, k - 1) == kBoundary) 
                {
                    mSystem.b(i, j, k) += c.z * f(i, j, k - 1)[component];
                }
            }
        });
}


