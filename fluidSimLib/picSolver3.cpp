//---picSolver3.cpp

#include "picSolver3.h"
#include "mathUtil.h"

picSolver3::picSolver3() : picSolver3({1, 1, 1}, {1, 1, 1}, {0, 0, 0})
{
}

picSolver3::picSolver3(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& gridOrigin)
    : gridFluidSolver3(resolution, gridSpacing, gridOrigin)
{
    auto grids = gridSystemData();
    mSignedDistanceFieldId = grids->addScalarGrid3Data(
        std::make_shared<cellCenteredScalarGrid3>(resolution, gridSpacing, gridOrigin),
        std::numeric_limits<double>::max());
    mParticles = std::make_shared<particleSystemData3>();
}

picSolver3::~picSolver3() {
}

scalarGrid3Ptr picSolver3::signedDistanceField() const
{
    return gridSystemData()->scalarGrid3DataAt(mSignedDistanceFieldId);
}

const particleSystemData3Ptr& picSolver3::particleSystemData() const {
    return mParticles;
}

const particleEmitter3Ptr& picSolver3::particleEmitter() const {
    return mParticleEmitter;
}

void picSolver3::setParticleEmitter(const particleEmitter3Ptr& newEmitter)
{
    mParticleEmitter = newEmitter;
    newEmitter->setTarget(mParticles);
}

void picSolver3::onInitialize()
{
    gridFluidSolver3::onInitialize();

    updateParticleEmitter(0.0);

}

void picSolver3::onBeginAdvanceTimeStep(double timeIntervalInSeconds)
{
    updateParticleEmitter(timeIntervalInSeconds);

    transferFromParticlesToGrids();

    buildSignedDistanceField();

    extrapolateVelocityToAir();

    applyBoundaryCondition();
}

void picSolver3::computeAdvection(double timeIntervalInSeconds)
{
    extrapolateVelocityToAir();

    applyBoundaryCondition();

    transferFromGridsToParticles();

    moveParticles(timeIntervalInSeconds);

}

scalarField3Ptr picSolver3::fluidSdf() const
{
    return signedDistanceField();
}

void picSolver3::transferFromParticlesToGrids() {
    const faceCenteredGrid3Ptr& flow = gridSystemData()->velocity();
    std::vector<vector3>& positions = mParticles->positions();
    std::vector<vector3>& velocities = mParticles->velocities();
    size_t numberOfParticles = mParticles->numberOfParticles();

    // Clear velocity to zero
    flow->fill(vector3());

    // Weighted-average velocity
    //auto u = flow->uAccessor();
    //auto v = flow->vAccessor();
    //auto w = flow->wAccessor();
    dataBuffer3 uWeight(flow->uData().size());
    dataBuffer3 vWeight(flow->vData().size());
    dataBuffer3 wWeight(flow->wData().size());
    mMarkersU.resize(flow->uData().size());
    mMarkersV.resize(flow->vData().size());
    mMarkersW.resize(flow->wData().size());
    mMarkersU.set(0);
    mMarkersV.set(0);
    mMarkersW.set(0);
    LinearBufferSampler uSampler(
        &flow->uData(), flow->gridSpacing(), flow->uOrigin() );
    LinearBufferSampler vSampler(
        &flow->vData(), flow->gridSpacing(), flow->vOrigin() );
    LinearBufferSampler wSampler(
        &flow->wData(), flow->gridSpacing(), flow->wOrigin() );
    /*
    LinearArraySampler3<double, double> uSampler(
    flow->uConstAccessor(),
    flow->gridSpacing(),
    flow->uOrigin());
    LinearArraySampler3<double, double> vSampler(
    flow->vConstAccessor(),
    flow->gridSpacing(),
    flow->vOrigin());
    LinearArraySampler3<double, double> wSampler(
    flow->wConstAccessor(),
    flow->gridSpacing(),
    flow->wOrigin());
    */
    std::array<size3, 8> indices;
    std::array<double, 8> weights;
    for (size_t i = 0; i < numberOfParticles; ++i)
    {
        uSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 8; ++j) {
            flow->u(indices[j]) += velocities[i].x * weights[j];
            uWeight(indices[j]) += weights[j];
            mMarkersU(indices[j]) = 1;
        }

        vSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 8; ++j) {
            flow->v(indices[j]) += velocities[i].y * weights[j];
            vWeight(indices[j]) += weights[j];
            mMarkersV(indices[j]) = 1;
        }

        wSampler.getCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 8; ++j) {
            flow->w(indices[j]) += velocities[i].z * weights[j];
            wWeight(indices[j]) += weights[j];
            mMarkersW(indices[j]) = 1;
        }
    }

#ifdef _OPENMP
    uWeight.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
        if (uWeight(i, j, k) > 0.0) {
            flow->u(i, j, k) /= uWeight(i, j, k);
        }
    });
    vWeight.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
        if (vWeight(i, j, k) > 0.0) {
            flow->v(i, j, k) /= vWeight(i, j, k);
        }
    });
    wWeight.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
        if (wWeight(i, j, k) > 0.0) {
            flow->w(i, j, k) /= wWeight(i, j, k);
        }
    });
#else
    uWeight.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (uWeight(i, j, k) > 0.0) {
            flow->u(i, j, k) /= uWeight(i, j, k);
        }
    });
    vWeight.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (vWeight(i, j, k) > 0.0) {
            flow->v(i, j, k) /= vWeight(i, j, k);
        }
    });
    wWeight.forEachIndex([&](size_t i, size_t j, size_t k) {
        if (wWeight(i, j, k) > 0.0) {
            flow->w(i, j, k) /= wWeight(i, j, k);
        }
    });
#endif
}

void picSolver3::transferFromGridsToParticles() {
    const faceCenteredGrid3Ptr& flow = gridSystemData()->velocity();
    std::vector<vector3>& positions = mParticles->positions();
    std::vector<vector3>& velocities = mParticles->velocities();
    size_t numberOfParticles = mParticles->numberOfParticles();

    //parallelFor(kZeroSize, numberOfParticles, [&](size_t i) {
    //    velocities[i] = flow->sample(positions[i]);
    //    });
    for ( size_t i = 0; i < numberOfParticles; ++i )
    {
        velocities[i] = flow->sample(positions[i]);
    }
}

void picSolver3::moveParticles(double timeIntervalInSeconds)
{
    const faceCenteredGrid3Ptr& flow = gridSystemData()->velocity();
    std::vector<vector3>& positions = mParticles->positions();
    std::vector<vector3>& velocities = mParticles->velocities();
    size_t numberOfParticles = mParticles->numberOfParticles();
    int domainBoundaryFlag = closedDomainBoundaryFlag();
    boundingBox3 boundingBox = flow->boundingBox();

#ifdef _OPENMP
#pragma omp parallel for
    for ( int i = 0; i < (int)numberOfParticles; ++i )
#else
    for ( size_t i = 0; i < numberOfParticles; ++i )
#endif
    {
        vector3 pt0 = positions[i];
        vector3 pt1 = pt0;
        vector3 vel = velocities[i];

        // Adaptive time-stepping
        unsigned int numSubSteps
            = static_cast<unsigned int>(std::max(maxCfl(), 1.0));
        double dt = timeIntervalInSeconds / numSubSteps;
        for (unsigned int t = 0; t < numSubSteps; ++t) {
            vector3 vel0 = flow->sample(pt0);

            // Mid-point rule
            vector3 midPt = pt0 + 0.5 * dt * vel0;
            vector3 midVel = flow->sample(midPt);
            pt1 = pt0 + dt * midVel;

            pt0 = pt1;
        }

        if ((domainBoundaryFlag & kDirectionLeft)
            && pt1.x <= boundingBox.lowerCorner.x) {
            pt1.x = boundingBox.lowerCorner.x;
            vel.x = 0.0;
        }
        if ((domainBoundaryFlag & kDirectionRight)
            && pt1.x >= boundingBox.upperCorner.x) {
            pt1.x = boundingBox.upperCorner.x;
            vel.x = 0.0;
        }
        if ((domainBoundaryFlag & kDirectionDown)
            && pt1.y <= boundingBox.lowerCorner.y) {
            pt1.y = boundingBox.lowerCorner.y;
            vel.y = 0.0;
        }
        if ((domainBoundaryFlag & kDirectionUp)
            && pt1.y >= boundingBox.upperCorner.y) {
            pt1.y = boundingBox.upperCorner.y;
            vel.y = 0.0;
        }
        if ((domainBoundaryFlag & kDirectionBack)
            && pt1.z <= boundingBox.lowerCorner.z) {
            pt1.z = boundingBox.lowerCorner.z;
            vel.z = 0.0;
        }
        if ((domainBoundaryFlag & kDirectionFront)
            && pt1.z >= boundingBox.upperCorner.z) {
            pt1.z = boundingBox.upperCorner.z;
            vel.z = 0.0;
        }

        positions[i] = pt1;
        velocities[i] = vel;
    }

    collider3Ptr col = collider();
    if (col != nullptr) {
        for ( size_t i = 0; i < numberOfParticles; ++i )
        {
            col->resolveCollision(
                0.0,
                0.0,
                &positions[i],
                &velocities[i]);
        }

        /* parallelFor(
        kZeroSize,
        numberOfParticles,
        [&](size_t i) {
        col->resolveCollision(
        0.0,
        0.0,
        &positions[i],
        &velocities[i]);
        });*/
    }
}

void picSolver3::extrapolateVelocityToAir()
{
    const faceCenteredGrid3Ptr& vel = gridSystemData()->velocity();


    unsigned int depth = static_cast<unsigned int>(std::ceil(maxCfl()));
    mathUtil::extrapolateToRegion(vel->uData(), mMarkersU, depth, vel->uData());
    mathUtil::extrapolateToRegion(vel->vData(), mMarkersV, depth, vel->vData());
    mathUtil::extrapolateToRegion(vel->wData(), mMarkersW, depth, vel->wData());
}

void picSolver3::buildSignedDistanceField() {
    auto sdf = signedDistanceField();
    auto sdfPos = sdf->dataPosition();
    double maxH = std::max(
        std::max( sdf->gridSpacing().x, sdf->gridSpacing().y), sdf->gridSpacing().z);
    double radius = 1.2 * maxH / std::sqrt(2.0);
    double sdfBandRadius = 2.0 * radius;

    mParticles->buildNeighborSearcher(2 * radius);
    auto searcher = mParticles->neighborSearcher();
#ifdef _OPENMP
    sdf->forEachDataPointIndexOpenMP([&] (size_t i, size_t j, size_t k) {
#else
    sdf->forEachDataPointIndex([&] (size_t i, size_t j, size_t k) {
#endif
        vector3 pt = sdfPos(i, j, k);
        double minDist = sdfBandRadius;
        searcher->forEachNearbyPoint(
            pt, sdfBandRadius, [&] (size_t, const vector3& x) {
            minDist = std::min(minDist, pt.distanceTo(x));
        });
        (*sdf)(i, j, k) = minDist - radius;
    });

    extrapolateIntoCollider(sdf.get());
    }

void picSolver3::updateParticleEmitter(double timeIntervalInSeconds) {
    if (mParticleEmitter != nullptr) {
        mParticleEmitter->update(currentTimeInSeconds(), timeIntervalInSeconds);
    }
}

