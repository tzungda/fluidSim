//---mathUtil.h
#ifndef mathUtil_H
#define mathUtil_H

#include <cmath>
#include <basetsd.h>
#include <limits>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "vector3.h"
#include "dataBuffer3.h"
#include "markers3.h"
#include "vecDataBuffer3.h"

class mathUtil
{
public:
    static FloatType eps( )
    { return std::numeric_limits<FloatType>::epsilon(); }
    static FloatType maxFloat( )
    { return std::numeric_limits<FloatType>::max(); }
    static FloatType minFloat( )
    { return std::numeric_limits<FloatType>::min(); }
    static bool isInsideSdf( FloatType phi )
    { return phi < 0.0;}
    static FloatType square(FloatType x)
    { return x * x; }
    static FloatType cubic(FloatType x)
    { return x * x * x; }
    static FloatType pi( )
    { return (FloatType)3.14159265358979323846264338327950288; }
    static FloatType invPi( )
    { return (FloatType)1.0/(FloatType)3.14159265358979323846264338327950288; }

    //kPiF = 3.14159265358979323846264338327950288f;

    static int sign( FloatType x );

    static vector3 projectAndApplyFriction( const vector3& vel, const vector3& normal, FloatType frictionCoefficient );

    static FloatType fractionInsideSdf( FloatType phi0, FloatType phi1 );

    static FloatType fractionInside( FloatType phiBottomLeft, FloatType phiBottomRight, FloatType phiTopLeft, FloatType phiTopRight );

    static void cycleArray(FloatType* arr, int size) ;

    static FloatType monotonicCatmullRom( const FloatType& f0,
        const FloatType& f1,
        const FloatType& f2,
        const FloatType& f3,
        FloatType f );
    static vector3 monotonicCatmullRom( const vector3& f0,
        const vector3& f1,
        const vector3& f2,
        const vector3& f3,
        FloatType f );

    static FloatType clamp(FloatType val, FloatType low, FloatType high);
    static vector3 clamp( const vector3& val, const vector3& low, const vector3& high);

    static void extrapolateToRegion( const dataBuffer3 &input, const markers3& valid, unsigned int numberOfIterations, dataBuffer3 &output );
    static void extrapolateToRegion( const vecDataBuffer3 &input, const markers3& valid, unsigned int numberOfIterations, vecDataBuffer3 &output );

    static void getBarycentric(
        FloatType x,
        SSIZE_T iLow,
        SSIZE_T iHigh,
        SSIZE_T* i,
        FloatType* f);

    static FloatType lerp(const FloatType& a, const FloatType& b, FloatType c);

    static FloatType bilerp( const FloatType& a00, const FloatType& a10, const FloatType& a01, const FloatType& a11,
        FloatType c0, FloatType c1 );

    static FloatType trilerp(
        const FloatType& a000,
        const FloatType& a100,
        const FloatType& a010,
        const FloatType& a110,
        const FloatType& a001,
        const FloatType& a101,
        const FloatType& a011,
        const FloatType& a111,
        FloatType c0,
        FloatType c1,
        FloatType c2);

    static FloatType absmax( FloatType x, FloatType y );

    static FloatType smearedHeavisideSdf( FloatType phi );

    static vector3 uniformSampleSphere( FloatType u1, FloatType u2 );


};

#endif
