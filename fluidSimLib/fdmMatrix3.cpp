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

fdmMatrix3::fdmMatrix3( SizeType width, SizeType height, SizeType depth, const fdmMatrixRow3& initValue )
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
    SizeType iMin = std::min(size.x, mSize.x);
    SizeType jMin = std::min(size.y, mSize.y);
    SizeType kMin = std::min(size.z, mSize.z);
    for (SizeType k = 0; k < kMin; ++k)
    {
        for (SizeType j = 0; j < jMin; ++j)
        {
            for (SizeType i = 0; i < iMin; ++i) 
            {
                tempBuffer(i, j, k) = valueByIndex(i, j, k);
            }
        }
    }

    swap(tempBuffer);
}

void fdmMatrix3::resize( SizeType width, SizeType height, SizeType depth, const fdmMatrixRow3& initValue )
{
    resize( size3(width, height, depth), initValue );
}

const fdmMatrixRow3& fdmMatrix3::valueByIndex( SizeType i, SizeType j, SizeType k ) const
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

fdmMatrixRow3& fdmMatrix3::valueByIndex( SizeType i, SizeType j, SizeType k )
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

SizeType fdmMatrix3::width( ) const
{
    return mSize.x;
}

SizeType fdmMatrix3::height( ) const
{
    return mSize.y;
}

SizeType fdmMatrix3::depth( ) const
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


fdmMatrixRow3& fdmMatrix3::operator()(SizeType i, SizeType j, SizeType k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const fdmMatrixRow3& fdmMatrix3::operator()( SizeType i, SizeType j, SizeType k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}


