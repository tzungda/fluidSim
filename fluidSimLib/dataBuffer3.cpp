//---dataBuffer3.cpp

#include <algorithm>
#include "dataBuffer3.h"

dataBuffer3::dataBuffer3()
{
}

dataBuffer3::dataBuffer3( const size3& size, const FloatType initValue )
{
    resize(size, initValue);
}

dataBuffer3::dataBuffer3( SizeType width, SizeType height, SizeType depth, const FloatType initValue )
{
    resize(width, height, depth, initValue);
}


void dataBuffer3::resize( const size3& size, const FloatType initValue )
{
    dataBuffer3 tempBuffer;
    tempBuffer.mData.resize( (SizeType)size.x * size.y * size.z, initValue );
    tempBuffer.mSize = size;
    SizeType iMin = (SizeType)std::min(size.x, mSize.x);
    SizeType jMin = (SizeType)std::min(size.y, mSize.y);
    SizeType kMin = (SizeType)std::min(size.z, mSize.z);
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

void dataBuffer3::resize( SizeType width, SizeType height, SizeType depth, const FloatType initValue )
{
    resize( size3(width, height, depth), initValue );
}

const FloatType dataBuffer3::valueByIndex( SizeType i, SizeType j, SizeType k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

FloatType& dataBuffer3::valueByIndex( SizeType i, SizeType j, SizeType k )
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

void dataBuffer3::set( FloatType value )
{
    for ( FloatType &v: mData )
    {
        v = value;
    }
}

void dataBuffer3::set( const dataBuffer3& other )
{
    mData.resize(other.mData.size());
    std::copy(other.mData.begin(), other.mData.end(), mData.begin());
    mSize = other.mSize;
}

size3 dataBuffer3::size( ) const
{
    return mSize;
}

SizeType dataBuffer3::width( ) const
{
    return mSize.x;
}

SizeType dataBuffer3::height( ) const
{
    return mSize.y;
}

SizeType dataBuffer3::depth( ) const
{
    return mSize.z;
}

FloatType *dataBuffer3::data( )
{
    return mData.data();
}

std::vector<FloatType>::iterator dataBuffer3::begin()
{
    return mData.begin();
}

std::vector<FloatType>::const_iterator dataBuffer3::begin() const
{
    return mData.begin();
}

std::vector<FloatType>::iterator dataBuffer3::end()
{
    return mData.end();
}

std::vector<FloatType>::const_iterator dataBuffer3::end() const
{
    return mData.end();
}

void dataBuffer3::swap( dataBuffer3& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}


FloatType& dataBuffer3::operator()(SizeType i, SizeType j, SizeType k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const FloatType& dataBuffer3::operator()( SizeType i, SizeType j, SizeType k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

FloatType& dataBuffer3::operator()( const size3& pt )
{
    return mData[pt.x + mSize.x * (pt.y + mSize.y * pt.z)];
}

const FloatType& dataBuffer3::operator()( const size3& pt ) const
{
    return mData[pt.x + mSize.x * (pt.y + mSize.y * pt.z)];
}
