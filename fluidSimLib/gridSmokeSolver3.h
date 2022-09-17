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

    double smokeDiffusionCoefficient() const;

    void setSmokeDiffusionCoefficient(double newValue);

    double temperatureDiffusionCoefficient() const;

    void setTemperatureDiffusionCoefficient(double newValue);

    double buoyancySmokeDensityFactor() const;

    void setBuoyancySmokeDensityFactor(double newValue);

    double buoyancyTemperatureFactor() const;

    void setBuoyancyTemperatureFactor(double newValue);

    double smokeDecayFactor() const;

    void setSmokeDecayFactor(double newValue);

    double smokeTemperatureDecayFactor() const;

    void setTemperatureDecayFactor(double newValue);

    scalarGrid3Ptr smokeDensity() const;

    scalarGrid3Ptr temperature() const;


protected:
    void onEndAdvanceTimeStep(double timeIntervalInSeconds) override;

    void computeExternalForces(double timeIntervalInSeconds) override;

private:
    size_t mSmokeDensityDataId;
    size_t mTemperatureDataId;
    double mSmokeDiffusionCoefficient = 0.0;
    double mTemperatureDiffusionCoefficient = 0.0;
    double mBuoyancySmokeDensityFactor = -0.000625;
    double mBuoyancyTemperatureFactor = 5.0;
    double mSmokeDecayFactor = 0.001;
    double mTemperatureDecayFactor = 0.001;

    void computeDiffusion(double timeIntervalInSeconds);

    void computeBuoyancyForce(double timeIntervalInSeconds);
};

typedef std::shared_ptr<gridSmokeSolver3> gridSmokeSolver3Ptr;

#endif
