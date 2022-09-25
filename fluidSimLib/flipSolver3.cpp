//---flipSolver3.cpp
#include "flipSolver3.h"

#ifdef SELFDEFINED_USE_TBB
#include <tbb/parallel_for.h>
#include <tbb/blocked_range3d.h>
#endif

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
#ifdef SELFDEFINED_USE_TBB

    // u-vel
    size3 size = flow->uData().size();
    size_t len = size.x * size.y * size.z;
    tbb::parallel_for(
        tbb::blocked_range<size_t>( 0, len ),
        [this, flow](const tbb::blocked_range<size_t>& r)
    {
        for (size_t i=r.begin();i<r.end();++i) 
            mDelta.uData().data()[i] = flow->uData().data()[i] - mDelta.uData().data()[i];
    }
    );

    // v-vel
    size = flow->vData().size();
    len = size.x * size.y * size.z;
    tbb::parallel_for(
        tbb::blocked_range<size_t>( 0, len ),
        [this, flow](const tbb::blocked_range<size_t>& r)
    {
        for (size_t i=r.begin();i<r.end();++i) 
            mDelta.vData().data()[i] = flow->vData().data()[i] - mDelta.vData().data()[i];
    }
    );

    // w-vel
    size = flow->wData().size();
    len = size.x * size.y * size.z;
    tbb::parallel_for(
        tbb::blocked_range<size_t>( 0, len ),
        [this, flow](const tbb::blocked_range<size_t>& r)
    {
        for (size_t i=r.begin();i<r.end();++i) 
            mDelta.wData().data()[i] = flow->wData().data()[i] - mDelta.wData().data()[i];
    }
    );

    // Transfer delta to the particles
    tbb::parallel_for(
        tbb::blocked_range<size_t>( 0, numberOfParticles ),
        [this, &velocities, &positions](const tbb::blocked_range<size_t>& r)
    {
        for (size_t i=r.begin();i<r.end();++i) 
            velocities[i] += mDelta.sample(positions[i]);
    }
    );


#else
#ifdef _OPENMP
    flow->forEachUIndexOpenMP([&](size_t i, size_t j, size_t k)
#else
    flow->forEachUIndex([&](size_t i, size_t j, size_t k)
#endif
    {
        mDelta.u(i, j, k) = flow->u(i, j, k) - mDelta.u(i, j, k);
    });

#ifdef _OPENMP
    flow->forEachVIndexOpenMP([&](size_t i, size_t j, size_t k)
#else
    flow->forEachVIndex([&](size_t i, size_t j, size_t k)
#endif
    {
        mDelta.v(i, j, k) = flow->v(i, j, k) - mDelta.v(i, j, k);
    });

#ifdef _OPENMP
    flow->forEachWIndexOpenMP([&](size_t i, size_t j, size_t k)
#else
    flow->forEachWIndex([&](size_t i, size_t j, size_t k)
#endif
    {
        mDelta.w(i, j, k) = flow->w(i, j, k) - mDelta.w(i, j, k);
    });

    // Transfer delta to the particles
#ifdef _OPENMP
    for ( int i = 0; i < (int)numberOfParticles; ++i )
#else
    for ( size_t i = 0; i < numberOfParticles; ++i )
#endif
    {
        velocities[i] += mDelta.sample(positions[i]);
    }
#endif
}

