//---flipSolver3.cpp
#include "flipSolver3.h"

flipSolver3::flipSolver3() : flipSolver3({1, 1, 1}, {1, 1, 1}, {0, 0, 0})
{
}

flipSolver3::flipSolver3(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& gridOrigin)
    : picSolver3(resolution, gridSpacing, gridOrigin)
{
}

flipSolver3::~flipSolver3()
{
}

void flipSolver3::transferFromParticlesToGrids()
{
    picSolver3::transferFromParticlesToGrids();

    mDelta.set(*gridSystemData()->velocity());
}

void flipSolver3::transferFromGridsToParticles()
{
    const faceCenteredGrid3Ptr& flow = gridSystemData()->velocity();
    std::vector<vector3>& positions = particleSystemData()->positions();
    std::vector<vector3>& velocities = particleSystemData()->velocities();
    size_t numberOfParticles = particleSystemData()->numberOfParticles();

    // Compute delta
    flow->forEachUIndex([&](size_t i, size_t j, size_t k)
    {
        mDelta.u(i, j, k) = flow->u(i, j, k) - mDelta.u(i, j, k);
    });

    flow->forEachVIndex([&](size_t i, size_t j, size_t k)
    {
        mDelta.v(i, j, k) = flow->v(i, j, k) - mDelta.v(i, j, k);
    });

    flow->forEachWIndex([&](size_t i, size_t j, size_t k)
    {
        mDelta.w(i, j, k) = flow->w(i, j, k) - mDelta.w(i, j, k);
    });

    // Transfer delta to the particles
    //parallelFor(kZeroSize, numberOfParticles, [&](size_t i) {
    //    velocities[i] += mDelta.sample(positions[i]);
    //});
    for ( size_t i = 0; i < numberOfParticles; ++i )
    {
        velocities[i] += mDelta.sample(positions[i]);
    }
}
