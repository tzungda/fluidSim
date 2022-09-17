//---fdmMatrix3.cpp

#include <algorithm>
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
    size_t iMin = std::min(size.x, mSize.x);
    size_t jMin = std::min(size.y, mSize.y);
    size_t kMin = std::min(size.z, mSize.z);
    for (size_t k = 0; k < kMin; ++k)
    {
        for (size_t j = 0; j < jMin; ++j)
        {
            for (size_t i = 0; i < iMin; ++i) 
            {
                tempBuffer(i, j, k) = valueByIndex(i, j, k);
            }
        }
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


