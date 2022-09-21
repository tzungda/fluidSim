//---cubicSemiLagrangian3.h
#ifndef cubicSemiLagrangian3_H
#define cubicSemiLagrangian3_H

#include <functional>

#include "semiLagrangian3.h"

class cubicSemiLagrangian3 : public semiLagrangian3
{
public:
    cubicSemiLagrangian3();

protected:

    std::function<FloatType(const vector3&)>
        getScalarSamplerFunc(const scalarGrid3& source) const override;


    std::function<vector3(const vector3&)>
        getVectorSamplerFunc(const extendVectorGrid3& source) const override;


    std::function<vector3(const vector3&)>
        getVectorSamplerFunc(const faceCenteredGrid3& source) const override;
};

typedef std::shared_ptr<cubicSemiLagrangian3> cubicSemiLagrangian3Ptr;

#endif

