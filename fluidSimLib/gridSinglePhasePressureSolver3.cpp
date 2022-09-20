//---gridSinglePhasePressureSolver3.cpp

#include "gridSinglePhasePressureSolver3.h"
#include "fdmIccgSolver3.h"
#include "gridBlockedBoundaryConditionSolver3.h"

const char kFluid = 0;
const char kAir = 1;
const char kBoundary = 2;

const double kDefaultTolerance = 1e-6f;

gridSinglePhasePressureSolver3::gridSinglePhasePressureSolver3() 
{
    mSystemSolver = std::make_shared<fdmIccgSolver3>(100, kDefaultTolerance);
}

gridSinglePhasePressureSolver3::~gridSinglePhasePressureSolver3() 
{
}

void gridSinglePhasePressureSolver3::solve(
    const faceCenteredGrid3& input,
    double timeIntervalInSeconds,
    faceCenteredGrid3* output,
    const scalarField3& boundarySdf,
    const vectorField3& boundaryVelocity,
    const scalarField3& fluidSdf) 
{

    auto pos = input.cellCenterPosition();
    buildMarkers(
        input.resolution(),
        pos,
        boundarySdf,
        fluidSdf);
    buildSystem(input);

    if (mSystemSolver != nullptr) 
    {
        // solve the system
        mSystemSolver->solve(&mSystem);

        // apply pressure gradient
        applyPressureGradient(input, output);
    }
}

gridBoundaryConditionSolver3Ptr gridSinglePhasePressureSolver3::suggestedBoundaryConditionSolver() const 
{
    return std::make_shared<gridBlockedBoundaryConditionSolver3>();
}

void gridSinglePhasePressureSolver3::setLinearSystemSolver( const fdmLinearSystemSolver3Ptr& solver) 
{
    mSystemSolver = solver;
}

const dataBuffer3& gridSinglePhasePressureSolver3::pressure() const
{
    return mSystem.x;
}

void gridSinglePhasePressureSolver3::buildMarkers(
    const size3& size,
    const std::function<vector3(size_t, size_t, size_t)>& pos,
    const scalarField3& boundarySdf,
    const scalarField3& fluidSdf)
{
    mMarkers.resize(size);
    mMarkers.forEachIndex([&](size_t i, size_t j, size_t k) {
        vector3 pt = pos(i, j, k);
        if (mathUtil::isInsideSdf(boundarySdf.sample(pt))) 
        {
            mMarkers(i, j, k) = kBoundary;
        } 
        else if (mathUtil::isInsideSdf(fluidSdf.sample(pt))) 
        {
            mMarkers(i, j, k) = kFluid;
        } 
        else
        {
            mMarkers(i, j, k) = kAir;
        }
        });
}

void gridSinglePhasePressureSolver3::buildSystem(
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

        if (mMarkers(i, j, k) == kFluid)
        {
            mSystem.b(i, j, k) = input.divergenceAtCellCenterByIndex(i, j, k);

            if (i + 1 < size.x && mMarkers(i + 1, j, k) != kBoundary) 
            {
                row.center += invHSqr.x;
                if (mMarkers(i + 1, j, k) == kFluid) 
                {
                    row.right -= invHSqr.x;
                }
            }

            if (i > 0 && mMarkers(i - 1, j, k) != kBoundary) 
            {
                row.center += invHSqr.x;
            }

            if (j + 1 < size.y && mMarkers(i, j + 1, k) != kBoundary) 
            {
                row.center += invHSqr.y;
                if (mMarkers(i, j + 1, k) == kFluid) 
                {
                    row.up -= invHSqr.y;
                }
            }

            if (j > 0 && mMarkers(i, j - 1, k) != kBoundary) 
            {
                row.center += invHSqr.y;
            }

            if (k + 1 < size.z && mMarkers(i, j, k + 1) != kBoundary) 
            {
                row.center += invHSqr.z;
                if (mMarkers(i, j, k + 1) == kFluid) 
                {
                    row.front -= invHSqr.z;
                }
            }

            if (k > 0 && mMarkers(i, j, k - 1) != kBoundary) 
            {
                row.center += invHSqr.z;
            }
        } 
        else
        {
            row.center = 1.0;
        }
        });
}

void gridSinglePhasePressureSolver3::applyPressureGradient(
    const faceCenteredGrid3& input,
    faceCenteredGrid3* output) 
{
    size3 size = input.resolution();

    vector3 invH = 1.0 / input.gridSpacing();

    mSystem.x.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (mMarkers(i, j, k) == kFluid) {
            if (i + 1 < size.x && mMarkers(i + 1, j, k) != kBoundary) 
            {
                output->u( i, j, k )
                    = input.u( i, j, k )
                    + invH.x
                    * (mSystem.x(i + 1, j, k) - mSystem.x(i, j, k));
            }
            if (j + 1 < size.y && mMarkers(i, j + 1, k) != kBoundary) 
            {
                output->v( i, j, k )
                    = input.v( i, j, k )
                    + invH.y
                    * (mSystem.x(i, j + 1, k) - mSystem.x(i, j, k));
            }
            if (k + 1 < size.z && mMarkers(i, j, k + 1) != kBoundary) 
            {
                output->w( i, j, k )
                    = input.w( i, j, k )
                    + invH.z
                    * (mSystem.x(i, j, k + 1) - mSystem.x(i, j, k));
            }
        }
        });
}


