//---dataBufferSamplers.cpp

#include <basetsd.h>

#include "mathUtil.h"
#include "dataBufferSamplers.h"

LinearBufferSampler::LinearBufferSampler()
{
}

LinearBufferSampler::LinearBufferSampler( dataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin )
{
    mGridSpacing = gridSpacing;
    mInvGridSpacing.x = (FloatType)1.0 / mGridSpacing.x;
    mInvGridSpacing.y = (FloatType)1.0 / mGridSpacing.y;
    mInvGridSpacing.z = (FloatType)1.0 / mGridSpacing.z;
    mOrigin = gridOrigin;
    mDataBuffer = dataBuffer;
}

LinearBufferSampler::LinearBufferSampler( const LinearBufferSampler& other )
{
    mGridSpacing = other.mGridSpacing;
    mInvGridSpacing = other.mInvGridSpacing;
    mOrigin = other.mOrigin;
    mDataBuffer = other.mDataBuffer;
}

// return sampled value at the given point
FloatType LinearBufferSampler::operator()( const vector3& pt ) const
{
    SSIZE_T i, j, k;
    FloatType cx, cy, cz;

    vector3 tmp( pt.x - mOrigin.x, pt.y - mOrigin.y, pt.z - mOrigin.z );
    vector3 normalizedX( tmp.x / mGridSpacing.x, tmp.y / mGridSpacing.y, tmp.z / mGridSpacing.z );

    SSIZE_T isize = mDataBuffer->size().x;
    SSIZE_T jsize = mDataBuffer->size().y;
    SSIZE_T ksize = mDataBuffer->size().z;

    mathUtil::getBarycentric( normalizedX.x, 0, isize - 1, &i, &cx );
    mathUtil::getBarycentric( normalizedX.y, 0, jsize - 1, &j, &cy );
    mathUtil::getBarycentric( normalizedX.z, 0, ksize - 1, &k, &cz );

    SSIZE_T i1 = std::min(i + 1, isize - 1);
    SSIZE_T j1 = std::min(j + 1, jsize - 1);
    SSIZE_T k1 = std::min(k + 1, ksize - 1);

   
    return mathUtil::trilerp( (*mDataBuffer)( i, j, k ),
                            (*mDataBuffer)( i1, j, k ),
                            (*mDataBuffer)(i, j1, k),
                            (*mDataBuffer)(i1, j1, k),
                            (*mDataBuffer)(i, j, k1),
                            (*mDataBuffer)(i1, j, k1),
                            (*mDataBuffer)(i, j1, k1),
                            (*mDataBuffer)(i1, j1, k1),
                            cx,
                            cy,
                            cz);
}

// return the indices of points and their sampling weight for given point.
void LinearBufferSampler::getCoordinatesAndWeights( const vector3& pt, std::array<size3, 8>* indices, std::array<FloatType, 8>* weights) const
{
    SSIZE_T i, j, k;
    FloatType fx, fy, fz;

    const vector3 normalizedX( (pt.x - mOrigin.x)*mInvGridSpacing.x, (pt.y - mOrigin.y)*mInvGridSpacing.y, (pt.z - mOrigin.z)*mInvGridSpacing.z );

    const SSIZE_T isize = static_cast<SSIZE_T>(mDataBuffer->size().x);
    const SSIZE_T jsize = static_cast<SSIZE_T>(mDataBuffer->size().y);
    const SSIZE_T ksize = static_cast<SSIZE_T>(mDataBuffer->size().z);

    mathUtil::getBarycentric(normalizedX.x, 0, isize - 1, &i, &fx);
    mathUtil::getBarycentric(normalizedX.y, 0, jsize - 1, &j, &fy);
    mathUtil::getBarycentric(normalizedX.z, 0, ksize - 1, &k, &fz);

    const SSIZE_T i1 = std::min(i + 1, isize - 1);
    const SSIZE_T j1 = std::min(j + 1, jsize - 1);
    const SSIZE_T k1 = std::min(k + 1, ksize - 1);


    (*indices)[0] = size3(i, j, k);
    (*indices)[1] = size3(i1, j, k);
    (*indices)[2] = size3(i, j1, k);
    (*indices)[3] = size3(i1, j1, k);
    (*indices)[4] = size3(i, j, k1);
    (*indices)[5] = size3(i1, j, k1);
    (*indices)[6] = size3(i, j1, k1);
    (*indices)[7] = size3(i1, j1, k1);

    (*weights)[0] = ((FloatType)1.0 - fx) * ((FloatType)1.0 - fy) * ((FloatType)1.0 - fz);
    (*weights)[1] = fx * ((FloatType)1.0 - fy) * ((FloatType)1.0 - fz);
    (*weights)[2] = ((FloatType)1.0 - fx) * fy * ((FloatType)1.0 - fz);
    (*weights)[3] = fx * fy * ((FloatType)1.0 - fz);
    (*weights)[4] = ((FloatType)1.0 - fx) * ((FloatType)1.0 - fy) * fz;
    (*weights)[5] = fx * ((FloatType)1.0 - fy) * fz;
    (*weights)[6] = ((FloatType)1.0 - fx) * fy * fz;
    (*weights)[7] = fx * fy * fz;
}

std::function<FloatType(const vector3&)> LinearBufferSampler::functor() const
{
    LinearBufferSampler sampler(*this);
    return std::bind(
        &LinearBufferSampler::operator(), sampler, std::placeholders::_1);
}


//==========================================================================
LinearVecBufferSampler::LinearVecBufferSampler()
{
}

LinearVecBufferSampler::LinearVecBufferSampler( vecDataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin )
{
    mGridSpacing = gridSpacing;
    mInvGridSpacing.x = (FloatType)1.0 / mGridSpacing.x;
    mInvGridSpacing.y = (FloatType)1.0 / mGridSpacing.y;
    mInvGridSpacing.z = (FloatType)1.0 / mGridSpacing.z;
    mOrigin = gridOrigin;
    mDataBuffer = dataBuffer;
}

LinearVecBufferSampler::LinearVecBufferSampler( const LinearVecBufferSampler& other )
{
    mGridSpacing = other.mGridSpacing;
    mInvGridSpacing = other.mInvGridSpacing;
    mOrigin = other.mOrigin;
    mDataBuffer = other.mDataBuffer;
}

// return sampled value at the given point
vector3 LinearVecBufferSampler::operator()( const vector3& pt ) const
{
    SSIZE_T i, j, k;
    FloatType cx, cy, cz;

    vector3 tmp( pt.x - mOrigin.x, pt.y - mOrigin.y, pt.z - mOrigin.z );
    vector3 normalizedX( tmp.x / mGridSpacing.x, tmp.y / mGridSpacing.y, tmp.z / mGridSpacing.z );

    SSIZE_T isize = mDataBuffer->size().x;
    SSIZE_T jsize = mDataBuffer->size().y;
    SSIZE_T ksize = mDataBuffer->size().z;

    mathUtil::getBarycentric( normalizedX.x, 0, isize - 1, &i, &cx );
    mathUtil::getBarycentric( normalizedX.y, 0, jsize - 1, &j, &cy );
    mathUtil::getBarycentric( normalizedX.z, 0, ksize - 1, &k, &cz );

    SSIZE_T i1 = std::min(i + 1, isize - 1);
    SSIZE_T j1 = std::min(j + 1, jsize - 1);
    SSIZE_T k1 = std::min(k + 1, ksize - 1);

    vector3 v_i_j_k = (*mDataBuffer)( i, j, k );
    vector3 v_i1_j_k = (*mDataBuffer)( i1, j, k );
    vector3 v_i_j1_k = (*mDataBuffer)( i, j1, k );
    vector3 v_i1_j1_k = (*mDataBuffer)( i1, j1, k );
    vector3 v_i_j_k1 = (*mDataBuffer)( i, j, k1 );
    vector3 v_i1_j_k1 = (*mDataBuffer)( i1, j, k1 );
    vector3 v_i_j1_k1 = (*mDataBuffer)( i, j1, k1 );
    vector3 v_i1_j1_k1 = (*mDataBuffer)( i1, j1, k1 );

    FloatType resX = mathUtil::trilerp( v_i_j_k.x,
        v_i1_j_k.x,
        v_i_j1_k.x,
        v_i1_j1_k.x,
        v_i_j_k1.x,
        v_i1_j_k1.x,
        v_i_j1_k1.x,
        v_i1_j1_k1.x,
        cx,
        cy,
        cz);
    FloatType resY = mathUtil::trilerp( v_i_j_k.y,
        v_i1_j_k.y,
        v_i_j1_k.y,
        v_i1_j1_k.y,
        v_i_j_k1.y,
        v_i1_j_k1.y,
        v_i_j1_k1.y,
        v_i1_j1_k1.y,
        cx,
        cy,
        cz);
    FloatType resZ = mathUtil::trilerp( v_i_j_k.z,
        v_i1_j_k.z,
        v_i_j1_k.z,
        v_i1_j1_k.z,
        v_i_j_k1.z,
        v_i1_j_k1.z,
        v_i_j1_k1.z,
        v_i1_j1_k1.z,
        cx,
        cy,
        cz);

    return vector3( resX, resY, resZ );
}

// return the indices of points and their sampling weight for given point.
void LinearVecBufferSampler::getCoordinatesAndWeights( const vector3& pt, std::array<size3, 8>* indices, std::array<FloatType, 8>* weights) const
{
    SSIZE_T i, j, k;
    FloatType fx, fy, fz;

    const vector3 normalizedX( (pt.x - mOrigin.x)*mInvGridSpacing.x, (pt.y - mOrigin.y)*mInvGridSpacing.y, (pt.z - mOrigin.z)*mInvGridSpacing.z );

    const SSIZE_T isize = static_cast<SSIZE_T>(mDataBuffer->size().x);
    const SSIZE_T jsize = static_cast<SSIZE_T>(mDataBuffer->size().y);
    const SSIZE_T ksize = static_cast<SSIZE_T>(mDataBuffer->size().z);

    mathUtil::getBarycentric(normalizedX.x, 0, isize - 1, &i, &fx);
    mathUtil::getBarycentric(normalizedX.y, 0, jsize - 1, &j, &fy);
    mathUtil::getBarycentric(normalizedX.z, 0, ksize - 1, &k, &fz);

    const SSIZE_T i1 = std::min(i + 1, isize - 1);
    const SSIZE_T j1 = std::min(j + 1, jsize - 1);
    const SSIZE_T k1 = std::min(k + 1, ksize - 1);


    (*indices)[0] = size3(i, j, k);
    (*indices)[1] = size3(i1, j, k);
    (*indices)[2] = size3(i, j1, k);
    (*indices)[3] = size3(i1, j1, k);
    (*indices)[4] = size3(i, j, k1);
    (*indices)[5] = size3(i1, j, k1);
    (*indices)[6] = size3(i, j1, k1);
    (*indices)[7] = size3(i1, j1, k1);

    (*weights)[0] = (1 - fx) * (1 - fy) * (1 - fz);
    (*weights)[1] = fx * (1 - fy) * (1 - fz);
    (*weights)[2] = (1 - fx) * fy * (1 - fz);
    (*weights)[3] = fx * fy * (1 - fz);
    (*weights)[4] = (1 - fx) * (1 - fy) * fz;
    (*weights)[5] = fx * (1 - fy) * fz;
    (*weights)[6] = (1 - fx) * fy * fz;
    (*weights)[7] = fx * fy * fz;
}

std::function<vector3(const vector3&)> LinearVecBufferSampler::functor() const
{
    LinearVecBufferSampler sampler(*this);
    return std::bind(
        &LinearVecBufferSampler::operator(), sampler, std::placeholders::_1);
}


//=========================================================
cubicBufferSamplerVec::cubicBufferSamplerVec(
    const vecDataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin)
{
    mGridSpacing = gridSpacing;
    mInvGridSpacing.x = (FloatType)1.0 / mGridSpacing.x;
    mInvGridSpacing.y = (FloatType)1.0 / mGridSpacing.y;
    mInvGridSpacing.z = (FloatType)1.0 / mGridSpacing.z;
    mOrigin = gridOrigin;
    mDataBuffer = dataBuffer;
}


cubicBufferSamplerVec::cubicBufferSamplerVec(
    const cubicBufferSamplerVec& other)
{
    mGridSpacing = other.mGridSpacing;
    mInvGridSpacing = other.mInvGridSpacing;
    mOrigin = other.mOrigin;
    mDataBuffer = other.mDataBuffer;
}

vector3 cubicBufferSamplerVec::operator()(const vector3& x) const {
    SSIZE_T i, j, k;
    SSIZE_T iSize = static_cast<SSIZE_T>(mDataBuffer->size().x);
    SSIZE_T jSize = static_cast<SSIZE_T>(mDataBuffer->size().y);
    SSIZE_T kSize = static_cast<SSIZE_T>(mDataBuffer->size().z);
    FloatType fx, fy, fz;

    SSIZE_T kZeroSSize = 0;
    vector3 normalizedX = (x - mOrigin) / mGridSpacing;

    mathUtil::getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    mathUtil::getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
    mathUtil::getBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

    SSIZE_T is[4] = {
        std::max(i - 1, kZeroSSize),
        i,
        std::min(i + 1, iSize - 1),
        std::min(i + 2, iSize - 1)
    };
    SSIZE_T js[4] = {
        std::max(j - 1, kZeroSSize),
        j,
        std::min(j + 1, jSize - 1),
        std::min(j + 2, jSize - 1)
    };
    SSIZE_T ks[4] = {
        std::max(k - 1, kZeroSSize),
        k,
        std::min(k + 1, kSize - 1),
        std::min(k + 2, kSize - 1)
    };

    vector3 kValues[4];

    for (int kk = 0; kk < 4; ++kk) 
    {
        vector3 jValues[4];

        for (int jj = 0; jj < 4; ++jj) 
        {
            jValues[jj] = mathUtil::monotonicCatmullRom(
                (*mDataBuffer)(is[0], js[jj], ks[kk]),
                (*mDataBuffer)(is[1], js[jj], ks[kk]),
                (*mDataBuffer)(is[2], js[jj], ks[kk]),
                (*mDataBuffer)(is[3], js[jj], ks[kk]),
                fx);
        }

        kValues[kk] = mathUtil::monotonicCatmullRom(
            jValues[0], jValues[1], jValues[2], jValues[3], fy);
    }

    return mathUtil::monotonicCatmullRom(
        kValues[0], kValues[1], kValues[2], kValues[3], fz);
}

std::function<vector3(const vector3&)> cubicBufferSamplerVec::functor() const {
    cubicBufferSamplerVec sampler(*this);
    return std::bind(
        &cubicBufferSamplerVec::operator(), sampler, std::placeholders::_1);
}


//=========================================================
cubicBufferSamplerScalar::cubicBufferSamplerScalar( const dataBuffer3* dataBuffer, const vector3 &gridSpacing, const vector3& gridOrigin)
{
    mGridSpacing = gridSpacing;
    mInvGridSpacing.x = (FloatType)1.0 / mGridSpacing.x;
    mInvGridSpacing.y = (FloatType)1.0 / mGridSpacing.y;
    mInvGridSpacing.z = (FloatType)1.0 / mGridSpacing.z;
    mOrigin = gridOrigin;
    mDataBuffer = dataBuffer;
}


cubicBufferSamplerScalar::cubicBufferSamplerScalar(const cubicBufferSamplerScalar& other)
{
    mGridSpacing = other.mGridSpacing;
    mInvGridSpacing = other.mInvGridSpacing;
    mOrigin = other.mOrigin;
    mDataBuffer = other.mDataBuffer;
}


FloatType cubicBufferSamplerScalar::operator()(const vector3& pt) const
{
    SSIZE_T i, j, k;
    SSIZE_T iSize = static_cast<SSIZE_T>(mDataBuffer->size().x);
    SSIZE_T jSize = static_cast<SSIZE_T>(mDataBuffer->size().y);
    SSIZE_T kSize = static_cast<SSIZE_T>(mDataBuffer->size().z);
    FloatType fx, fy, fz;

    SSIZE_T kZeroSSize = 0;
    vector3 normalizedX = (pt - mOrigin) / mGridSpacing;

    mathUtil::getBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    mathUtil::getBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
    mathUtil::getBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

    SSIZE_T is[4] = 
    {
        std::max(i - 1, kZeroSSize),
        i,
        std::min(i + 1, iSize - 1),
        std::min(i + 2, iSize - 1)
    };
    SSIZE_T js[4] = 
    {
        std::max(j - 1, kZeroSSize),
        j,
        std::min(j + 1, jSize - 1),
        std::min(j + 2, jSize - 1)
    };
    SSIZE_T ks[4] = 
    {
        std::max(k - 1, kZeroSSize),
        k,
        std::min(k + 1, kSize - 1),
        std::min(k + 2, kSize - 1)
    };

    FloatType kValues[4];

    for (int kk = 0; kk < 4; ++kk) 
    {
        FloatType jValues[4];

        for (int jj = 0; jj < 4; ++jj) 
        {
            jValues[jj] = mathUtil::monotonicCatmullRom(
                (*mDataBuffer)(is[0], js[jj], ks[kk]),
                (*mDataBuffer)(is[1], js[jj], ks[kk]),
                (*mDataBuffer)(is[2], js[jj], ks[kk]),
                (*mDataBuffer)(is[3], js[jj], ks[kk]),
                fx);
        }

        kValues[kk] = mathUtil::monotonicCatmullRom(
            jValues[0], jValues[1], jValues[2], jValues[3], fy);
    }

    return mathUtil::monotonicCatmullRom(
        kValues[0], kValues[1], kValues[2], kValues[3], fz);
}


std::function<FloatType(const vector3&)> cubicBufferSamplerScalar::functor() const
{
    cubicBufferSamplerScalar sampler(*this);
    return std::bind(
        &cubicBufferSamplerScalar::operator(), sampler, std::placeholders::_1);
}

