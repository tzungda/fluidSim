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

vecDataBuffer3::vecDataBuffer3( size_t width, size_t height, size_t depth, const vector3 initValue )
{
    resize(width, height, depth, initValue);
}


void vecDataBuffer3::resize( const size3& size, const vector3 initValue )
{
    vecDataBuffer3 tempBuffer;
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

void vecDataBuffer3::resize( size_t width, size_t height, size_t depth, const vector3 initValue )
{
    resize( size3(width, height, depth), initValue );
}

const vector3 vecDataBuffer3::valueByIndex( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

vector3& vecDataBuffer3::valueByIndex( size_t i, size_t j, size_t k )
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

size3 vecDataBuffer3::size( ) const
{
    return mSize;
}

size_t vecDataBuffer3::width( ) const
{
    return mSize.x;
}

size_t vecDataBuffer3::height( ) const
{
    return mSize.y;
}

size_t vecDataBuffer3::depth( ) const
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


vector3& vecDataBuffer3::operator()(size_t i, size_t j, size_t k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}


const vector3& vecDataBuffer3::operator()(size_t i, size_t j, size_t k) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}


