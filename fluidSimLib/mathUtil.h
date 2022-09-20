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
    static double eps( )
    { return std::numeric_limits<double>::epsilon(); }
    static double maxFloat( )
    { return std::numeric_limits<double>::max(); }
    static double minFloat( )
    { return std::numeric_limits<double>::min(); }
    static bool isInsideSdf( double phi )
    { return phi < 0.0;}
    static double square(double x)
    { return x * x; }
    static double cubic(double x)
    { return x * x * x; }
    static double pi( )
    { return 3.14159265358979323846264338327950288; }
    static double invPi( )
    { return 1.0/3.14159265358979323846264338327950288; }

    //kPiF = 3.14159265358979323846264338327950288f;

    static int sign( double x );

    static vector3 projectAndApplyFriction( const vector3& vel, const vector3& normal, double frictionCoefficient );

    static double fractionInsideSdf( double phi0, double phi1 );

    static double fractionInside( double phiBottomLeft, double phiBottomRight, double phiTopLeft, double phiTopRight );

    static void cycleArray(double* arr, int size) ;

    static double monotonicCatmullRom( const double& f0,
        const double& f1,
        const double& f2,
        const double& f3,
        double f );
    static vector3 monotonicCatmullRom( const vector3& f0,
        const vector3& f1,
        const vector3& f2,
        const vector3& f3,
        double f );

    static double clamp(double val, double low, double high);
    static vector3 clamp( const vector3& val, const vector3& low, const vector3& high);

    static void extrapolateToRegion( const dataBuffer3 &input, const markers3& valid, unsigned int numberOfIterations, dataBuffer3 &output );
    static void extrapolateToRegion( const vecDataBuffer3 &input, const markers3& valid, unsigned int numberOfIterations, vecDataBuffer3 &output );

    static void getBarycentric(
        double x,
        SSIZE_T iLow,
        SSIZE_T iHigh,
        SSIZE_T* i,
        double* f);

    static double lerp(const double& a, const double& b, double c);

    static double bilerp( const double& a00, const double& a10, const double& a01, const double& a11,
        double c0, double c1 );

    static double trilerp(
        const double& a000,
        const double& a100,
        const double& a010,
        const double& a110,
        const double& a001,
        const double& a101,
        const double& a011,
        const double& a111,
        double c0,
        double c1,
        double c2);

    static double absmax( double x, double y );

    static double smearedHeavisideSdf( double phi );

    static vector3 uniformSampleSphere( double u1, double u2 );


};

#endif
