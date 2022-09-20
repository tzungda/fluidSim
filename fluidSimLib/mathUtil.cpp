//---mathUtil.cpp

#include <cmath>
#include <algorithm>

#include "mathUtil.h"

int mathUtil::sign( double x )
{
    if ( x >= 0.0 )
    {
        return 1;
    } else {
        return -1;
    }
}

vector3 mathUtil::projectAndApplyFriction( const vector3& vel, const vector3& normal, double frictionCoefficient )
{
    vector3 velt = vel.projected(normal);
    if (velt.lengthSquared() > 0.0)
    {
        double veln = std::fmax(-vel.dot(normal), 0.0);
        velt *= std::fmax(1.0 - frictionCoefficient * veln / velt.length(), 0.0);
    }

    return velt;
}

double mathUtil::fractionInsideSdf( double phi0, double phi1 )
{
    if (isInsideSdf(phi0) && isInsideSdf(phi1))
    {
        return 1.0;
    }
    else if (isInsideSdf(phi0) && !isInsideSdf(phi1))
    {
        return phi0 / (phi0 - phi1);
    }
    else if (!isInsideSdf(phi0) && isInsideSdf(phi1))
    {
        return phi1 / (phi1 - phi0);
    }
    else
    {
        return 0.0;
    }
}

double mathUtil::fractionInside( double phiBottomLeft, double phiBottomRight, double phiTopLeft, double phiTopRight )
{
    int inside_count = (phiBottomLeft < 0 ? 1 : 0) + (phiTopLeft < 0 ? 1 : 0) +
        (phiBottomRight < 0 ? 1 : 0) + (phiTopRight < 0 ? 1 : 0);
    double list[] = {phiBottomLeft, phiBottomRight, phiTopRight, phiTopLeft};

    if (inside_count == 4)
    {
        return 1;
    }
    else if (inside_count == 3)
    {
        // rotate until the positive value is in the first position
        while (list[0] < 0)
        {
            cycleArray(list, 4);
        }

        // Work out the area of the exterior triangle
        double side0 = 1 - fractionInsideSdf(list[0], list[3]);
        double side1 = 1 - fractionInsideSdf(list[0], list[1]);
        return 1 - 0.5 * side0 * side1;
    }
    else if (inside_count == 2)
    {
        // rotate until a negative value is in the first position, and the next
        // negative is in either slot 1 or 2.
        while (list[0] >= 0 || !(list[1] < 0 || list[2] < 0))
        {
            cycleArray(list, 4);
        }

        if (list[1] < 0)
        {  // the matching signs are adjacent
            double side_left = fractionInsideSdf(list[0], list[3]);
            double side_right = fractionInsideSdf(list[1], list[2]);
            return 0.5 * (side_left + side_right);
        }
        else
        {  // matching signs are diagonally opposite
           // determine the centre point's sign to disambiguate this case
            double middle_point = 0.25f * (list[0] + list[1] + list[2] + list[3]);
            if (middle_point < 0)
            {
                double area = 0.0;

                // first triangle (top left)
                double side1 = 1.0 - fractionInsideSdf(list[0], list[3]);
                double side3 = 1.0 - fractionInsideSdf(list[2], list[3]);

                area += 0.5 * side1 * side3;

                // second triangle (top right)
                double side2 = 1.0 - fractionInsideSdf(list[2], list[1]);
                double side0 = 1.0 - fractionInsideSdf(list[0], list[1]);
                area += 0.5 * side0 * side2;

                return 1.0 - area;
            }
            else
            {
                double area = 0.0;

                // first triangle (bottom left)
                double side0 = fractionInsideSdf(list[0], list[1]);
                double side1 = fractionInsideSdf(list[0], list[3]);
                area += 0.5 * side0 * side1;

                // second triangle (top right)
                double side2 = fractionInsideSdf(list[2], list[1]);
                double side3 = fractionInsideSdf(list[2], list[3]);
                area += 0.5 * side2 * side3;
                return area;
            }
        }
    }
    else if (inside_count == 1)
    {
        // rotate until the negative value is in the first position
        while (list[0] >= 0.0)
        {
            cycleArray(list, 4);
        }

        // Work out the area of the interior triangle, and subtract from 1.
        double side0 = fractionInsideSdf(list[0], list[3]);
        double side1 = fractionInsideSdf(list[0], list[1]);
        return 0.5 * side0 * side1;
    }
    else
    {
        return 0.0;
    }
}

void mathUtil::cycleArray(double* arr, int size)
{
    double t = arr[0];
    for (int i = 0; i < size - 1; ++i) arr[i] = arr[i + 1];
    arr[size - 1] = t;
}

double mathUtil::monotonicCatmullRom( const double& f0,
    const double& f1,
    const double& f2,
    const double& f3,
    double f )
{
    double d1 = (f2 - f0) / 2.0;
    double d2 = (f3 - f1) / 2.0;
    double D1 = f2 - f1;

    if (std::fabs(D1) < eps() )
    {
        d1 = d2 = 0;
    }

    if (sign(D1) != sign(d1))
    {
        d1 = 0;
    }

    if (sign(D1) != sign(d2))
    {
        d2 = 0;
    }

    double a3 = d1 + d2 - 2 * D1;
    double a2 = 3 * D1 - 2 * d1 - d2;
    double a1 = d1;
    double a0 = f1;

    return a3 * cubic(f) + a2 * square(f) + a1 * f + a0;
}

vector3 mathUtil::monotonicCatmullRom( const vector3& v0,
    const vector3& v1,
    const vector3& v2,
    const vector3& v3,
    double f )
{
    static const double two = 2.0;
    static const double three = 3.0;

    vector3 d1 = (v2 - v0) / two;
    vector3 d2 = (v3 - v1) / two;
    vector3 D1 = v2 - v1;

    if (std::fabs(D1.x) < std::numeric_limits<double>::epsilon() ||
        sign(D1.x) != sign(d1.x) ||
        sign(D1.x) != sign(d2.x))
    {
        d1.x = d2.x = 0;
    }

    if (std::fabs(D1.y) < std::numeric_limits<double>::epsilon() ||
        sign(D1.y) != sign(d1.y) ||
        sign(D1.y) != sign(d2.y))
    {
        d1.y = d2.y = 0;
    }

    if (std::fabs(D1.z) < std::numeric_limits<double>::epsilon() ||
        sign(D1.z) != sign(d1.z) ||
        sign(D1.z) != sign(d2.z))
    {
        d1.z = d2.z = 0;
    }

    vector3 a3 = d1 + d2 - two * D1;
    vector3 a2 = three * D1 - two * d1 - d2;
    vector3 a1 = d1;
    vector3 a0 = v1;

    return a3 * (f * f * f) + a2 * (f * f) + a1 * f + a0;
}

double mathUtil::clamp(double val, double low, double high)
{
    if (val < low)
    {
        return low;
    }
    else if (val > high)
    {
        return high;
    }
    else
    {
        return val;
    }
}

vector3 mathUtil::clamp( const vector3& val, const vector3& low, const vector3& high)
{
    return vector3(
        clamp(val.x, low.x, high.x),
        clamp(val.y, low.y, high.y),
        clamp(val.z, low.z, high.z));
}

void mathUtil::extrapolateToRegion( const dataBuffer3 &input, const markers3& valid, unsigned int numberOfIterations, dataBuffer3 &output )
{
    const size3 size = input.size();

    markers3 valid0(size);
    markers3 valid1(size);

    valid0.forEachIndex([&](size_t i, size_t j, size_t k) {
        valid0(i, j, k) = valid(i, j, k);
        output(i, j, k) = input(i, j, k);
    });

    for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
    {
        valid0.forEachIndex([&](size_t i, size_t j, size_t k) {
            double sum = 0.0;//zero<T>();
            unsigned int count = 0;

            if (!valid0(i, j, k))
            {
                if (i + 1 < size.x && valid0(i + 1, j, k))
                {
                    sum += output(i + 1, j, k);
                    ++count;
                }

                if (i > 0 && valid0(i - 1, j, k))
                {
                    sum += output(i - 1, j, k);
                    ++count;
                }

                if (j + 1 < size.y && valid0(i, j + 1, k))
                {
                    sum += output(i, j + 1, k);
                    ++count;
                }

                if (j > 0 && valid0(i, j - 1, k))
                {
                    sum += output(i, j - 1, k);
                    ++count;
                }

                if (k + 1 < size.z && valid0(i, j, k + 1))
                {
                    sum += output(i, j, k + 1);
                    ++count;
                }

                if (k > 0 && valid0(i, j, k - 1))
                {
                    sum += output(i, j, k - 1);
                    ++count;
                }

                if (count > 0)
                {
                    output(i, j, k)
                        = sum
                        / (double)(count);
                    valid1(i, j, k) = 1;
                }
            }
            else
            {
                valid1(i, j, k) = 1;
            }
        });

        valid0.swap(valid1);
    }
}

void mathUtil::extrapolateToRegion( const vecDataBuffer3 &input, const markers3& valid, unsigned int numberOfIterations, vecDataBuffer3 &output )
{
    const size3 size = input.size();

    markers3 valid0(size);
    markers3 valid1(size);

    valid0.forEachIndex([&](size_t i, size_t j, size_t k) {
        valid0(i, j, k) = valid(i, j, k);
        output(i, j, k) = input(i, j, k);
    });

    for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
    {
        valid0.forEachIndex([&](size_t i, size_t j, size_t k) {
            vector3 sum = vector3( 0.0, 0.0, 0.0 );
            unsigned int count = 0;

            if (!valid0(i, j, k))
            {
                if (i + 1 < size.x && valid0(i + 1, j, k))
                {
                    sum += output(i + 1, j, k);
                    ++count;
                }

                if (i > 0 && valid0(i - 1, j, k))
                {
                    sum += output(i - 1, j, k);
                    ++count;
                }

                if (j + 1 < size.y && valid0(i, j + 1, k))
                {
                    sum += output(i, j + 1, k);
                    ++count;
                }

                if (j > 0 && valid0(i, j - 1, k))
                {
                    sum += output(i, j - 1, k);
                    ++count;
                }

                if (k + 1 < size.z && valid0(i, j, k + 1))
                {
                    sum += output(i, j, k + 1);
                    ++count;
                }

                if (k > 0 && valid0(i, j, k - 1))
                {
                    sum += output(i, j, k - 1);
                    ++count;
                }

                if (count > 0)
                {
                    output(i, j, k)
                        = sum
                        / (double)(count);
                    valid1(i, j, k) = 1;
                }
            }
            else
            {
                valid1(i, j, k) = 1;
            }
        });

        valid0.swap(valid1);
    }
}

void mathUtil::getBarycentric(
    double x,
    SSIZE_T iLow,
    SSIZE_T iHigh,
    SSIZE_T* i,
    double* f)
{
    double s = std::floor(x);
    *i = static_cast<SSIZE_T>(s);

    SSIZE_T offset = -iLow;
    iLow += offset;
    iHigh += offset;

    if (iLow == iHigh)
    {
        *i = iLow;
        *f = 0;
    }
    else if (*i < iLow)
    {
        *i = iLow;
        *f = 0;
    }
    else if (*i > iHigh - 1)
    {
        *i = iHigh - 1;
        *f = 1;
    }
    else
    {
        *f = x - s;
    }

    *i -= offset;
}


double mathUtil::lerp(const double& a, const double& b, double c)
{
    return (1.0 - c) * a + c * b;
}


double mathUtil::bilerp( const double& a00, const double& a10, const double& a01, const double& a11,
    double c0, double c1 )
{
    return lerp(
        lerp(a00, a10, c0),
        lerp(a01, a11, c0),
        c1);
}


double mathUtil::trilerp(
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
    double c2)
{
    return lerp(
        bilerp(a000, a100, a010, a110, c0, c1),
        bilerp(a001, a101, a011, a111, c0, c1),
        c2);
}

double mathUtil::absmax( double x, double y )
{
    return (x*x > y*y) ? x : y;
}

double mathUtil::smearedHeavisideSdf( double phi )
{
    if (phi > 1.5)
    {
        return 1.0;
    }
    else
    {
        if (phi < -1.5)
        {
            return 0.0;
        }
        else
        {
            return 0.5 + phi / 3.0 +
                0.5 * invPi() * std::sin( pi() * phi / 1.5);
        }
    }
}

vector3 mathUtil::uniformSampleSphere( double u1, double u2 )
{
    double y = 1 - 2 * u1;
    double r = std::sqrt(std::max<double>(0, 1 - y * y));
    double phi = (2.0f * pi()) * u2;
    double x = r * std::cos(phi);
    double z = r * std::sin(phi);
    return vector3(x, y, z);
}
