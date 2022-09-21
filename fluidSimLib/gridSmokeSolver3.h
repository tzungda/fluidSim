//---gridSmokeSolver3.h
#ifndef gridSmokeSolver3_H
#define gridSmokeSolver3_H

#include "gridFluidSolver3.h"

class gridSmokeSolver3 : public gridFluidSolver3
{
public:
    gridSmokeSolver3();

    gridSmokeSolver3(
        const size3& resolution,
        const vector3& gridSpacing,
        const vector3& gridOrigin);

    virtual ~gridSmokeSolver3();

    FloatType smokeDiffusionCoefficient() const;

    void setSmokeDiffusionCoefficient(FloatType newValue);

    FloatType temperatureDiffusionCoefficient() const;

    void setTemperatureDiffusionCoefficient(FloatType newValue);

    FloatType buoyancySmokeDensityFactor() const;

    void setBuoyancySmokeDensityFactor(FloatType newValue);

    FloatType buoyancyTemperatureFactor() const;

    void setBuoyancyTemperatureFactor(FloatType newValue);

    FloatType smokeDecayFactor() const;

    void setSmokeDecayFactor(FloatType newValue);

    FloatType smokeTemperatureDecayFactor() const;

    void setTemperatureDecayFactor(FloatType newValue);

    scalarGrid3Ptr smokeDensity() const;

    scalarGrid3Ptr temperature() const;


protected:
    void onEndAdvanceTimeStep(FloatType timeIntervalInSeconds) override;

    void computeExternalForces(FloatType timeIntervalInSeconds) override;

private:
    size_t mSmokeDensityDataId;
    size_t mTemperatureDataId;
    FloatType mSmokeDiffusionCoefficient = 0.0;
    FloatType mTemperatureDiffusionCoefficient = 0.0;
    FloatType mBuoyancySmokeDensityFactor = (FloatType)-0.000625;
    FloatType mBuoyancyTemperatureFactor = (FloatType)5.0;
    FloatType mSmokeDecayFactor = (FloatType)0.001;
    FloatType mTemperatureDecayFactor = (FloatType)0.001;

    void computeDiffusion(FloatType timeIntervalInSeconds);

    void computeBuoyancyForce(FloatType timeIntervalInSeconds);
};

typedef std::shared_ptr<gridSmokeSolver3> gridSmokeSolver3Ptr;

#endif
