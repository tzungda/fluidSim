//---fdmMatrix3.cpp

#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "fdmMatrix3.h"

fdmMatrix3::fdmMatrix3()
{
}

fdmMatrix3::fdmMatrix3( const size3& size, const fdmMatrixRow3& initValue )
{
    resize(size, initValue);
}

fdmMatrix3::fdmMatrix3( size_t width, size_t height, size_t depth, const fdmMatrixRow3& initValue )
{
    resize(width, height, depth, initValue);
}

fdmMatrix3::fdmMatrix3( const fdmMatrix3& other )
{
    mSize = other.mSize;
    mData = other.mData;
}


void fdmMatrix3::resize( const size3& size, const fdmMatrixRow3& initValue )
{
    fdmMatrix3 tempBuffer;
    tempBuffer.mData.resize(size.x * size.y * size.z, initValue );
    tempBuffer.mSize = size;

    if ( size != mSize )
    {
        size_t iMin = std::min(size.x, mSize.x);
        size_t jMin = std::min(size.y, mSize.y);
        size_t kMin = std::min(size.z, mSize.z);
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for ( int k = 0; k < (int)kMin; ++k)
        {
            for ( int j = 0; j < (int)jMin; ++j)
            {
                for ( int i = 0; i < (int)iMin; ++i) 
                {
                    tempBuffer(i, j, k) = valueByIndex(i, j, k);
                }
            }
        }
    }
    else
    {
        if ( tempBuffer.mData.size() > 0 && mData.size() > 0 )
            memcpy( &tempBuffer.mData[0], &mData[0], sizeof( fdmMatrixRow3 )*mData.size() );
    }

    swap(tempBuffer);
}

void fdmMatrix3::resize( size_t width, size_t height, size_t depth, const fdmMatrixRow3& initValue )
{
    resize( size3(width, height, depth), initValue );
}

const fdmMatrixRow3& fdmMatrix3::valueByIndex( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

fdmMatrixRow3& fdmMatrix3::valueByIndex( size_t i, size_t j, size_t k )
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}


void fdmMatrix3::set( const fdmMatrixRow3& value )
{
    for ( fdmMatrixRow3 &v: mData )
    {
        v = value;
    }
}

void fdmMatrix3::set( const fdmMatrix3& other )
{
    mData.resize(other.mData.size());
    std::copy(other.mData.begin(), other.mData.end(), mData.begin());
    mSize = other.mSize;
}

size3 fdmMatrix3::size( ) const
{
    return mSize;
}

size_t fdmMatrix3::width( ) const
{
    return mSize.x;
}

size_t fdmMatrix3::height( ) const
{
    return mSize.y;
}

size_t fdmMatrix3::depth( ) const
{
    return mSize.z;
}

fdmMatrixRow3 *fdmMatrix3::data( )
{
    return mData.data();
}

const fdmMatrixRow3* fdmMatrix3::data( ) const
{
    return mData.data();
}

void fdmMatrix3::swap( fdmMatrix3& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}


fdmMatrixRow3& fdmMatrix3::operator()(size_t i, size_t j, size_t k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const fdmMatrixRow3& fdmMatrix3::operator()( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}


const fdmMatrixRow3* fdmMatrix3::iBuffer( size_t j, size_t k, size_t i ) const
{
    return &mData[i + mSize.x * (j + mSize.y * k)];
}

