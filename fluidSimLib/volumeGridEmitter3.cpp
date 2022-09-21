//---volumeGridEmitter3.cpp

#include "volumeGridEmitter3.h"
#include "faceCenteredGrid3.h"
#include "extendVectorGrid3.h"
#include "mathUtil.h"


volumeGridEmitter3::volumeGridEmitter3(
    const implicitSurface3Ptr& sourceRegion,
    bool isOneShot)
    : mSourceRegion(sourceRegion)
    , mIsOneShot(isOneShot)
{
}

volumeGridEmitter3::~volumeGridEmitter3()
{
}

void volumeGridEmitter3::addSignedDistanceTarget(
    const scalarGrid3Ptr& scalarGridTarget)
{
    auto mapper = [] (FloatType sdf, const vector3&, FloatType oldVal)
    {
        return std::min(oldVal, sdf);
    };
    addTarget(scalarGridTarget, mapper);
}

void volumeGridEmitter3::addStepFunctionTarget(
    const scalarGrid3Ptr& scalarGridTarget,
    FloatType minValue,
    FloatType maxValue)
{
    FloatType smoothingWidth = scalarGridTarget->gridSpacing().min();
    auto mapper = [minValue, maxValue, smoothingWidth, scalarGridTarget]
    (FloatType sdf, const vector3&, FloatType oldVal) {
        FloatType step = (FloatType)1.0 - mathUtil::smearedHeavisideSdf(sdf / smoothingWidth);
        return std::max(oldVal, (maxValue - minValue) * step + minValue);
    };
    addTarget(scalarGridTarget, mapper);
}

void volumeGridEmitter3::addTarget(
    const scalarGrid3Ptr& scalarGridTarget,
    const scalarMapper& customMapper)
{
    mCustomScalarTargets.emplace_back(scalarGridTarget, customMapper);
}

void volumeGridEmitter3::addTarget(
    const vectorGrid3Ptr& vectorGridTarget,
    const vectorMapper& customMapper)
{
    mCustomVectorTargets.emplace_back(vectorGridTarget, customMapper);
}

const implicitSurface3Ptr& volumeGridEmitter3::sourceRegion() const
{
    return mSourceRegion;
}

bool volumeGridEmitter3::isOneShot() const
{
    return mIsOneShot;
}

void volumeGridEmitter3::onUpdate(
    FloatType currentTimeInSeconds,
    FloatType timeIntervalInSeconds)
{
    if (mHasEmitted && mIsOneShot)
    {
        return;
    }

    emit();

    mHasEmitted = true;
}

void volumeGridEmitter3::emit()
{
    for (const auto& target : mCustomScalarTargets) {
        const auto& grid = std::get<0>(target);
        const auto& mapper = std::get<1>(target);

        auto pos = grid->dataPosition();
        grid->forEachDataPointIndex(
            [&] (size_t i, size_t j, size_t k) {
            vector3 gx = pos(i, j, k);
            FloatType sdf = sourceRegion()->signedDistance(gx);
            (*grid)(i, j, k) = mapper(sdf, gx, (*grid)(i, j, k));
        });
    }

    for (const auto& target : mCustomVectorTargets) {
        const auto& grid = std::get<0>(target);
        const auto& mapper = std::get<1>(target);

        extendVectorGrid3Ptr extendVecGrid
            = std::dynamic_pointer_cast<extendVectorGrid3>(grid);
        if (extendVecGrid != nullptr) {
            vectorGrid3::DataPositionFunc pos = extendVecGrid->dataPosition();
            extendVecGrid->forEachDataPointIndex(
                [&] (size_t i, size_t j, size_t k) {
                vector3 gx = pos(i, j, k);
                FloatType sdf = sourceRegion()->signedDistance(gx);
                if ( mathUtil::isInsideSdf(sdf)) {
                    (*extendVecGrid)(i, j, k)
                        = mapper(sdf, gx, (*extendVecGrid)(i, j, k));
                }
            });
            continue;
        }

        faceCenteredGrid3Ptr faceCentered = std::dynamic_pointer_cast<faceCenteredGrid3>(grid);
        if (faceCentered != nullptr)
        {
            vectorGrid3::DataPositionFunc uPos = faceCentered->uPosition();
            vectorGrid3::DataPositionFunc vPos = faceCentered->vPosition();
            vectorGrid3::DataPositionFunc wPos = faceCentered->wPosition();

            faceCentered->forEachUIndex(
                [&] (size_t i, size_t j, size_t k) {
                vector3 gx = uPos(i, j, k);
                FloatType sdf = sourceRegion()->signedDistance(gx);
                vector3 oldVal = faceCentered->sample(gx);
                vector3 newVal = mapper(sdf, gx, oldVal);
                faceCentered->u(i, j, k) = newVal.x;
            });
            faceCentered->forEachVIndex(
                [&] (size_t i, size_t j, size_t k) {
                vector3 gx = vPos(i, j, k);
                FloatType sdf = sourceRegion()->signedDistance(gx);
                vector3 oldVal = faceCentered->sample(gx);
                vector3 newVal = mapper(sdf, gx, oldVal);
                faceCentered->v(i, j, k) = newVal.y;
            });
            faceCentered->forEachWIndex(
                [&] (size_t i, size_t j, size_t k) {
                vector3 gx = wPos(i, j, k);
                FloatType sdf = sourceRegion()->signedDistance(gx);
                vector3 oldVal = faceCentered->sample(gx);
                vector3 newVal = mapper(sdf, gx, oldVal);
                faceCentered->w(i, j, k) = newVal.z;
            });
            continue;
        }
    }
}

