//---cubicSemiLagrangian3.cpp

#include "cubicSemiLagrangian3.h"
#include "dataBufferSamplers.h"

cubicSemiLagrangian3::cubicSemiLagrangian3() 
{
}

std::function<double(const vector3&)>
cubicSemiLagrangian3::getScalarSamplerFunc(const scalarGrid3& source) const
{
    auto sourceSampler = cubicBufferSamplerScalar( 
        &source.data(),
        source.gridSpacing(),
        source.dataOrigin());
    return sourceSampler.functor();
}

std::function<vector3(const vector3&)>
cubicSemiLagrangian3::getVectorSamplerFunc(
    const extendVectorGrid3& source) const
{
    auto sourceSampler = cubicBufferSamplerVec(
        &source.data(),
        source.gridSpacing(),
        source.dataOrigin());
    return sourceSampler.functor();
}

std::function<vector3(const vector3&)>
cubicSemiLagrangian3::getVectorSamplerFunc(
    const faceCenteredGrid3& source) const 
{
    auto uSourceSampler = cubicBufferSamplerScalar(
        &source.uData(),
        source.gridSpacing(),
        source.uOrigin());
    auto vSourceSampler = cubicBufferSamplerScalar(
        &source.vData(),
        source.gridSpacing(),
        source.vOrigin());
    auto wSourceSampler = cubicBufferSamplerScalar(
        &source.wData(),
        source.gridSpacing(),
        source.wOrigin());
    return
        [uSourceSampler, vSourceSampler, wSourceSampler](const vector3& x) 
    {
        return vector3(
            uSourceSampler(x), vSourceSampler(x), wSourceSampler(x));
    };
}


