//---dataBufferSamplers.h
#ifndef dataBufferSamplers_H
#define dataBufferSamplers_H

#include <functional>
#include <array>
#include "dataBuffer3.h"
#include "vecDataBuffer3.h"

//==========================================================================
class LinearBufferSampler
{
private:
    vector3 mGridSpacing;
    vector3 mInvGridSpacing;
    vector3 mOrigin;
    dataBuffer3 *mDataBuffer;
public:

    LinearBufferSampler();

    LinearBufferSampler(dataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin);

    LinearBufferSampler(const LinearBufferSampler& other);

    // return sampled value at the given point
    double operator()(const vector3& pt) const;

    // return the indices of points and their sampling weight for given point.
    void getCoordinatesAndWeights(const vector3& pt, std::array<size3, 8>* indices, std::array<double, 8>* weights) const;

    //! Returns a funtion object that wraps this instance.
    std::function<double(const vector3&)> functor() const;
};

//==========================================================================
class LinearVecBufferSampler
{
private:
    vector3 mGridSpacing;
    vector3 mInvGridSpacing;
    vector3 mOrigin;
    vecDataBuffer3 *mDataBuffer;
public:

    LinearVecBufferSampler();

    LinearVecBufferSampler(vecDataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin);

    LinearVecBufferSampler(const LinearVecBufferSampler& other);

    // return sampled value at the given point
    vector3 operator()(const vector3& pt) const;

    // return the indices of points and their sampling weight for given point.
    void getCoordinatesAndWeights(const vector3& pt, std::array<size3, 8>* indices, std::array<double, 8>* weights) const;

    //! Returns a funtion object that wraps this instance.
    std::function<vector3(const vector3&)> functor() const;
};


class cubicBufferSamplerVec
{
private:
    vector3 mGridSpacing;
    vector3 mInvGridSpacing;
    vector3 mOrigin;
    const vecDataBuffer3 *mDataBuffer;

public:
    cubicBufferSamplerVec(const vecDataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin);

    //! Copy constructor.
    cubicBufferSamplerVec(const cubicBufferSamplerVec& other);

    //! Returns sampled value at point \p pt.
    vector3 operator()(const vector3& pt) const;

    //! Returns a funtion object that wraps this instance.
    std::function<vector3(const vector3&)> functor() const;

};

class cubicBufferSamplerScalar
{
private:
    vector3 mGridSpacing;
    vector3 mInvGridSpacing;
    vector3 mOrigin;
    const dataBuffer3 *mDataBuffer;

public:
    cubicBufferSamplerScalar(const dataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin);

    //! Copy constructor.
    cubicBufferSamplerScalar(const cubicBufferSamplerScalar& other);

    //! Returns sampled value at point \p pt.
    double operator()(const vector3& pt) const;

    //! Returns a funtion object that wraps this instance.
    std::function<double(const vector3&)> functor() const;

};

#endif

