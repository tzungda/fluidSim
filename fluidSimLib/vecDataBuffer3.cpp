//---vecDataBuffer3.cpp

#include <algorithm>
#include "vecDataBuffer3.h"


vecDataBuffer3::vecDataBuffer3()
{
}

vecDataBuffer3::vecDataBuffer3( const size3& size, const vector3 initValue )
{
    resize(size, initValue);
}

vecDataBuffer3::vecDataBuffer3( SizeType width, SizeType height, SizeType depth, const vector3 initValue )
{
    resize(width, height, depth, initValue);
}


void vecDataBuffer3::resize( const size3& size, const vector3 initValue )
{
    vecDataBuffer3 tempBuffer;
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

void vecDataBuffer3::resize( SizeType width, SizeType height, SizeType depth, const vector3 initValue )
{
    resize( size3(width, height, depth), initValue );
}

const vector3 vecDataBuffer3::valueByIndex( SizeType i, SizeType j, SizeType k ) const
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

vector3& vecDataBuffer3::valueByIndex( SizeType i, SizeType j, SizeType k )
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

size3 vecDataBuffer3::size( ) const
{
    return mSize;
}

SizeType vecDataBuffer3::width( ) const
{
    return mSize.x;
}

SizeType vecDataBuffer3::height( ) const
{
    return mSize.y;
}

SizeType vecDataBuffer3::depth( ) const
{
    return mSize.z;
}

vector3 *vecDataBuffer3::data( )
{
    return mData.data();
}

void vecDataBuffer3::swap( vecDataBuffer3& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}

void vecDataBuffer3::set( const vecDataBuffer3& other )
{
    mData.resize(other.mData.size());
    std::copy(other.mData.begin(), other.mData.end(), mData.begin());
    mSize = other.mSize;
}


vector3& vecDataBuffer3::operator()(SizeType i, SizeType j, SizeType k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}


const vector3& vecDataBuffer3::operator()(SizeType i, SizeType j, SizeType k) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}


