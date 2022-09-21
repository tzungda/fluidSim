//---mathUtil.cpp

#include <cmath>
#include <algorithm>

#include "mathUtil.h"

int mathUtil::sign( FloatType x )
{
    if ( x >= 0.0 )
    {
        return 1;
    } else {
        return -1;
    }
}

vector3 mathUtil::projectAndApplyFriction( const vector3& vel, const vector3& normal, FloatType frictionCoefficient )
{
    vector3 velt = vel.projected(normal);
    if (velt.lengthSquared() > 0.0)
    {
        FloatType veln = std::fmax(-vel.dot(normal), (FloatType)0.0);
        velt *= std::fmax((FloatType)1.0 - frictionCoefficient * veln / velt.length(), (FloatType)0.0);
    }

    return velt;
}

FloatType mathUtil::fractionInsideSdf( FloatType phi0, FloatType phi1 )
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

FloatType mathUtil::fractionInside( FloatType phiBottomLeft, FloatType phiBottomRight, FloatType phiTopLeft, FloatType phiTopRight )
{
    int inside_count = (phiBottomLeft < 0 ? 1 : 0) + (phiTopLeft < 0 ? 1 : 0) +
        (phiBottomRight < 0 ? 1 : 0) + (phiTopRight < 0 ? 1 : 0);
    FloatType list[] = {phiBottomLeft, phiBottomRight, phiTopRight, phiTopLeft};

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
        FloatType side0 = (FloatType)1.0 - fractionInsideSdf(list[0], list[3]);
        FloatType side1 = (FloatType)1.0 - fractionInsideSdf(list[0], list[1]);
        return (FloatType)1.0 - (FloatType)0.5 * side0 * side1;
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
            FloatType side_left = fractionInsideSdf(list[0], list[3]);
            FloatType side_right = fractionInsideSdf(list[1], list[2]);
            return (FloatType)0.5 * (side_left + side_right);
        }
        else
        {  // matching signs are diagonally opposite
           // determine the centre point's sign to disambiguate this case
            FloatType middle_point = 0.25f * (list[0] + list[1] + list[2] + list[3]);
            if (middle_point < 0)
            {
                FloatType area = 0.0;

                // first triangle (top left)
                FloatType side1 = (FloatType)1.0 - fractionInsideSdf(list[0], list[3]);
                FloatType side3 = (FloatType)1.0 - fractionInsideSdf(list[2], list[3]);

                area += (FloatType)0.5 * side1 * side3;

                // second triangle (top right)
                FloatType side2 = (FloatType)1.0 - fractionInsideSdf(list[2], list[1]);
                FloatType side0 = (FloatType)1.0 - fractionInsideSdf(list[0], list[1]);
                area += (FloatType)0.5 * side0 * side2;

                return (FloatType)1.0 - area;
            }
            else
            {
                FloatType area = 0.0;

                // first triangle (bottom left)
                FloatType side0 = fractionInsideSdf(list[0], list[1]);
                FloatType side1 = fractionInsideSdf(list[0], list[3]);
                area += (FloatType)0.5 * side0 * side1;

                // second triangle (top right)
                FloatType side2 = fractionInsideSdf(list[2], list[1]);
                FloatType side3 = fractionInsideSdf(list[2], list[3]);
                area += (FloatType)0.5 * side2 * side3;
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
        FloatType side0 = fractionInsideSdf(list[0], list[3]);
        FloatType side1 = fractionInsideSdf(list[0], list[1]);
        return (FloatType)0.5 * side0 * side1;
    }
    else
    {
        return 0.0;
    }
}

void mathUtil::cycleArray(FloatType* arr, int size)
{
    FloatType t = arr[0];
    for (int i = 0; i < size - 1; ++i) arr[i] = arr[i + 1];
    arr[size - 1] = t;
}

FloatType mathUtil::monotonicCatmullRom( const FloatType& f0,
    const FloatType& f1,
    const FloatType& f2,
    const FloatType& f3,
    FloatType f )
{
    FloatType d1 = (f2 - f0) / (FloatType)2.0;
    FloatType d2 = (f3 - f1) / (FloatType)2.0;
    FloatType D1 = f2 - f1;

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

    FloatType a3 = d1 + d2 - 2 * D1;
    FloatType a2 = 3 * D1 - 2 * d1 - d2;
    FloatType a1 = d1;
    FloatType a0 = f1;

    return a3 * cubic(f) + a2 * square(f) + a1 * f + a0;
}

vector3 mathUtil::monotonicCatmullRom( const vector3& v0,
    const vector3& v1,
    const vector3& v2,
    const vector3& v3,
    FloatType f )
{
    static const FloatType two = 2.0;
    static const FloatType three = 3.0;

    vector3 d1 = (v2 - v0) / two;
    vector3 d2 = (v3 - v1) / two;
    vector3 D1 = v2 - v1;

    if (std::fabs(D1.x) < std::numeric_limits<FloatType>::epsilon() ||
        sign(D1.x) != sign(d1.x) ||
        sign(D1.x) != sign(d2.x))
    {
        d1.x = d2.x = 0;
    }

    if (std::fabs(D1.y) < std::numeric_limits<FloatType>::epsilon() ||
        sign(D1.y) != sign(d1.y) ||
        sign(D1.y) != sign(d2.y))
    {
        d1.y = d2.y = 0;
    }

    if (std::fabs(D1.z) < std::numeric_limits<FloatType>::epsilon() ||
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

FloatType mathUtil::clamp(FloatType val, FloatType low, FloatType high)
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

#ifdef _OPENMP
    valid0.forEachIndexOpenMP([&](size_t i, size_t j, size_t k) {
#else
    valid0.forEachIndex([&](size_t i, size_t j, size_t k) {
#endif
        valid0(i, j, k) = valid(i, j, k);
        output(i, j, k) = input(i, j, k);
    });

    for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
    {
        valid0.forEachIndex([&](size_t i, size_t j, size_t k) {
            FloatType sum = 0.0;//zero<T>();
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
                        / (FloatType)(count);
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
                        / (FloatType)(count);
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
    FloatType x,
    SSIZE_T iLow,
    SSIZE_T iHigh,
    SSIZE_T* i,
    FloatType* f)
{
    FloatType s = std::floor(x);
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


FloatType mathUtil::lerp(const FloatType& a, const FloatType& b, FloatType c)
{
    return ((FloatType)1.0 - c) * a + c * b;
}


FloatType mathUtil::bilerp( const FloatType& a00, const FloatType& a10, const FloatType& a01, const FloatType& a11,
    FloatType c0, FloatType c1 )
{
    return lerp(
        lerp(a00, a10, c0),
        lerp(a01, a11, c0),
        c1);
}


FloatType mathUtil::trilerp(
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
    FloatType c2)
{
    return lerp(
        bilerp(a000, a100, a010, a110, c0, c1),
        bilerp(a001, a101, a011, a111, c0, c1),
        c2);
}

FloatType mathUtil::absmax( FloatType x, FloatType y )
{
    return (x*x > y*y) ? x : y;
}

FloatType mathUtil::smearedHeavisideSdf( FloatType phi )
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
            return (FloatType)0.5 + phi / (FloatType)3.0 +
                (FloatType)0.5 * invPi() * std::sin( pi() * phi / (FloatType)1.5);
        }
    }
}

vector3 mathUtil::uniformSampleSphere( FloatType u1, FloatType u2 )
{
    FloatType y = 1 - 2 * u1;
    FloatType r = std::sqrt(std::max<FloatType>(0, 1 - y * y));
    FloatType phi = (2.0f * pi()) * u2;
    FloatType x = r * std::cos(phi);
    FloatType z = r * std::sin(phi);
    return vector3(x, y, z);
}

