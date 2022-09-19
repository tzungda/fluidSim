//---gridFractionalBoundaryConditionSolver3.cpp

#include "gridFractionalBoundaryConditionSolver3.h"
#include "surfaceToImplicit3.h"
#include "mathUtil.h"
#include "boundaryDef.h"

gridFractionalBoundaryConditionSolver3::gridFractionalBoundaryConditionSolver3() 
{
}

gridFractionalBoundaryConditionSolver3::~gridFractionalBoundaryConditionSolver3() 
{
}

void gridFractionalBoundaryConditionSolver3::constrainVelocity(
    faceCenteredGrid3* velocity,
    unsigned int extrapolationDepth) 
{
    size3 size = velocity->resolution();
    if (mColliderSdf.resolution() != size) 
    {
        updateCollider(
            collider(),
            size,
            velocity->gridSpacing(),
            velocity->origin());
    }

    auto uPos = velocity->uPosition();
    auto vPos = velocity->vPosition();
    auto wPos = velocity->wPosition();

    dataBuffer3 uTemp( velocity->uData().size() );
    dataBuffer3 vTemp( velocity->vData().size() );
    dataBuffer3 wTemp( velocity->wData().size() );
    markers3 uMarker( velocity->uData().size(), 1 );
    markers3 vMarker( velocity->vData().size(), 1 );
    markers3 wMarker( velocity->wData().size(), 1 );

    vector3 h = velocity->gridSpacing();

    // Assign collider's velocity first and initialize markers
    velocity->forEachUIndex([&](SizeType i, SizeType j, SizeType k) {
        vector3 pt = uPos(i, j, k);
        FloatType phi0 = mColliderSdf.sample(pt - vector3((FloatType)0.5 * h.x, (FloatType)0.0, (FloatType)0.0));
        FloatType phi1 = mColliderSdf.sample(pt + vector3((FloatType)0.5 * h.x, (FloatType)0.0, (FloatType)0.0));
        FloatType frac = mathUtil::fractionInsideSdf(phi0, phi1);
        frac = (FloatType)1.0 - mathUtil::clamp(frac, (FloatType)0.0, (FloatType)1.0);

        if (frac > 0.0) {
            uMarker(i, j, k) = 1;
        } else {
            vector3 colliderVel = collider()->velocityAt(pt);
            velocity->u(i, j, k ) = colliderVel.x;
            uMarker(i, j, k) = 0;
        }
        });

    velocity->forEachVIndex([&](SizeType i, SizeType j, SizeType k) {
        vector3 pt = vPos(i, j, k);
        FloatType phi0 = mColliderSdf.sample(pt - vector3((FloatType)0.0, (FloatType)0.5 * h.y, (FloatType)0.0));
        FloatType phi1 = mColliderSdf.sample(pt + vector3((FloatType)0.0, (FloatType)0.5 * h.y, (FloatType)0.0));
        FloatType frac = mathUtil::fractionInsideSdf(phi0, phi1);
        frac = (FloatType)1.0 - mathUtil::clamp(frac, (FloatType)0.0, (FloatType)1.0);

        if (frac > 0.0) {
            vMarker(i, j, k) = 1;
        } else {
            vector3 colliderVel = collider()->velocityAt(pt);
            velocity->v( i, j, k ) = colliderVel.y;
            vMarker(i, j, k) = 0;
        }
        });

    velocity->forEachWIndex([&](SizeType i, SizeType j, SizeType k) {
        vector3 pt = wPos(i, j, k);
        FloatType phi0 = mColliderSdf.sample(pt - vector3((FloatType)0.0, (FloatType)0.0, (FloatType)0.5 * h.z));
        FloatType phi1 = mColliderSdf.sample(pt + vector3((FloatType)0.0, (FloatType)0.0, (FloatType)0.5 * h.z));
        FloatType frac = mathUtil::fractionInsideSdf(phi0, phi1);
        frac = (FloatType)1.0 - mathUtil::clamp(frac, (FloatType)0.0, (FloatType)1.0);

        if (frac > 0.0) {
            wMarker(i, j, k) = 1;
        } else {
            vector3 colliderVel = collider()->velocityAt(pt);
            velocity->w( i, j, k ) = colliderVel.z;
            wMarker(i, j, k) = 0;
        }
        });

    // free-slip: extrapolate fluid velocity into the collider
    mathUtil::extrapolateToRegion(
        velocity->uData(), uMarker, extrapolationDepth, velocity->uData() );
    mathUtil::extrapolateToRegion(
        velocity->vData(), vMarker, extrapolationDepth, velocity->vData() );
    mathUtil::extrapolateToRegion(
        velocity->wData(), wMarker, extrapolationDepth, velocity->wData() );

    // no-flux: project the extrapolated velocity to the collider's surface
    // normal
    velocity->forEachUIndex([&](SizeType i, SizeType j, SizeType k) {
        vector3 pt = uPos(i, j, k);
        if ( mathUtil::isInsideSdf(mColliderSdf.sample(pt))) {
            vector3 colliderVel = collider()->velocityAt(pt);
            vector3 vel = velocity->sample(pt);
            vector3 g = mColliderSdf.gradient(pt);
            if (g.lengthSquared() > 0.0) {
                vector3 n = g.normalized();
                vector3 velr = vel - colliderVel;
                vector3 velt = mathUtil::projectAndApplyFriction(
                    velr, n, collider()->frictionCoefficient());

                vector3 velp = velt + colliderVel;
                uTemp(i, j, k) = velp.x;
            } else {
                uTemp(i, j, k) = colliderVel.x;
            }
        } else {
            uTemp(i, j, k) = velocity->u( i, j, k );
        }
        });

    velocity->forEachVIndex([&](SizeType i, SizeType j, SizeType k) {
        vector3 pt = vPos(i, j, k);
        if (mathUtil::isInsideSdf(mColliderSdf.sample(pt))) {
            vector3 colliderVel = collider()->velocityAt(pt);
            vector3 vel = velocity->sample(pt);
            vector3 g = mColliderSdf.gradient(pt);
            if (g.lengthSquared() > 0.0) {
                vector3 n = g.normalized();
                vector3 velr = vel - colliderVel;
                vector3 velt = mathUtil::projectAndApplyFriction(
                    velr, n, collider()->frictionCoefficient());

                vector3 velp = velt + colliderVel;
                vTemp(i, j, k) = velp.y;
            } else {
                vTemp(i, j, k) = colliderVel.y;
            }
        } else {
            vTemp(i, j, k) = velocity->v( i, j, k );
        }
        });

    velocity->forEachWIndex([&](SizeType i, SizeType j, SizeType k) {
        vector3 pt = wPos(i, j, k);
        if (mathUtil::isInsideSdf(mColliderSdf.sample(pt))) {
            vector3 colliderVel = collider()->velocityAt(pt);
            vector3 vel = velocity->sample(pt);
            vector3 g = mColliderSdf.gradient(pt);
            if (g.lengthSquared() > 0.0) {
                vector3 n = g.normalized();
                vector3 velr = vel - colliderVel;
                vector3 velt = mathUtil::projectAndApplyFriction(
                    velr, n, collider()->frictionCoefficient());

                vector3 velp = velt + colliderVel;
                wTemp(i, j, k) = velp.z;
            } else {
                wTemp(i, j, k) = colliderVel.z;
            }
        } else {
            wTemp(i, j, k) = velocity->w( i, j, k );
        }
        });

    // transfer results
    velocity->uData().forEachIndex([&](SizeType i, SizeType j, SizeType k) {
        velocity->u( i, j, k ) = uTemp(i, j, k);
        });
    velocity->vData().forEachIndex([&](SizeType i, SizeType j, SizeType k) {
        velocity->v( i, j, k ) = vTemp(i, j, k);
        });
    velocity->wData().forEachIndex([&](SizeType i, SizeType j, SizeType k) {
        velocity->w( i, j, k ) = wTemp(i, j, k);
        });

    // no-flux: Project velocity on the domain boundary if closed
    if (closedDomainBoundaryFlag() & kDirectionLeft) {
        for (SizeType k = 0; k < velocity->uData().size().z; ++k) 
        {
            for (SizeType j = 0; j < velocity->uData().size().y; ++j) 
            {
                velocity->u( 0, j, k ) = 0.0;
            }
        }
    }
    if (closedDomainBoundaryFlag() & kDirectionRight) 
    {
        for (SizeType k = 0; k < velocity->uData().size().z; ++k) 
        {
            for (SizeType j = 0; j < velocity->uData().size().y; ++j) 
            {
                velocity->u( velocity->uData().size().x - 1, j, k ) = 0.0;
            }
        }
    }
    if (closedDomainBoundaryFlag() & kDirectionDown) 
    {
        for (SizeType k = 0; k < velocity->vData().size().z; ++k) 
        {
            for (SizeType i = 0; i < velocity->vData().size().x; ++i) 
            {
                velocity->v( i, 0, k ) = 0.0;
            }
        }
    }
    if (closedDomainBoundaryFlag() & kDirectionUp) 
    {
        for (SizeType k = 0; k < velocity->vData().size().z; ++k) 
        {
            for (SizeType i = 0; i < velocity->vData().size().x; ++i) 
            {
                velocity->v( i, velocity->vData().size().y - 1, k ) = 0.0;
            }
        }
    }
    if (closedDomainBoundaryFlag() & kDirectionBack)
    {
        for (SizeType j = 0; j < velocity->wData().size().y; ++j) 
        {
            for (SizeType i = 0; i < velocity->wData().size().x; ++i) 
            {
                velocity->w( i, j, 0 ) = 0.0;
            }
        }
    }
    if (closedDomainBoundaryFlag() & kDirectionFront) 
    {
        for (SizeType j = 0; j < velocity->wData().size().y; ++j) 
        {
            for (SizeType i = 0; i < velocity->wData().size().x; ++i) 
            {
                velocity->w(i, j, velocity->wData().size().z - 1) = 0.0;
            }
        }
    }
}

const cellCenteredScalarGrid3&
gridFractionalBoundaryConditionSolver3::colliderSdf() const {
    return mColliderSdf;
}

void gridFractionalBoundaryConditionSolver3::onColliderUpdated(
    const size3& gridSize,
    const vector3& gridSpacing,
    const vector3& gridOrigin) 
{
    mColliderSdf.resize(gridSize, gridSpacing, gridOrigin);

    if (collider() != nullptr) 
    {
        surface3Ptr surface = collider()->surface();
        implicitSurface3Ptr implicitSurface
            = std::dynamic_pointer_cast<implicitSurface3>(surface);
        if (implicitSurface == nullptr) 
        {
            implicitSurface = std::make_shared<surfaceToImplicit3>(surface);
        }

        mColliderSdf.fill([&](const vector3& pt) {
            return implicitSurface->signedDistance(pt);
            });
    }
    else 
    {
        mColliderSdf.fill(mathUtil::maxFloat());
    }
}


