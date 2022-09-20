//---volumeGridEmitter3.h
#ifndef volumeGridEmitter3_H
#define volumeGridEmitter3_H

#include "gridEmitter3.h"
#include "implicitSurface3.h"
#include "scalarGrid3.h"
#include "vectorGrid3.h"

class volumeGridEmitter3 : public gridEmitter3
{
public:

    typedef std::function<double(double, const vector3&, double)>
        scalarMapper;

    typedef std::function<vector3(double, const vector3&, const vector3&)>
        vectorMapper;

    explicit volumeGridEmitter3(
        const implicitSurface3Ptr& sourceRegion,
        bool isOneShot = true);

    virtual ~volumeGridEmitter3();

    void addSignedDistanceTarget(const scalarGrid3Ptr& scalarGridTarget);

    void addStepFunctionTarget(
        const scalarGrid3Ptr& scalarGridTarget,
        double minValue,
        double maxValue);

    void addTarget(
        const scalarGrid3Ptr& scalarGridTarget,
        const scalarMapper& customMapper);

    void addTarget(
        const vectorGrid3Ptr& vectorGridTarget,
        const vectorMapper& customMapper);

    const implicitSurface3Ptr& sourceRegion() const;

    bool isOneShot() const;


private:
    typedef std::tuple<scalarGrid3Ptr, scalarMapper> scalarTarget;
    typedef std::tuple<vectorGrid3Ptr, vectorMapper> vectorTarget;

    implicitSurface3Ptr mSourceRegion;
    bool mIsOneShot = true;
    bool mHasEmitted = false;
    std::vector<scalarTarget> mCustomScalarTargets;
    std::vector<vectorTarget> mCustomVectorTargets;

    void onUpdate(
        double currentTimeInSeconds,
        double timeIntervalInSeconds) override;

    void emit();
};

typedef std::shared_ptr< volumeGridEmitter3 > volumeGridEmitter3Ptr;

#endif