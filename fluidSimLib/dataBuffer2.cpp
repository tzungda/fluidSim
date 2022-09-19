//---dataBuffer2.cpp

#include <algorithm>
#include "dataBuffer2.h"


dataBuffer2::dataBuffer2()
{
}

dataBuffer2::dataBuffer2( const size2& size, const FloatType initValue )
{
    resize(size, initValue);
}

dataBuffer2::dataBuffer2( SizeType width, SizeType height, const FloatType initValue )
{
    resize(width, height, initValue);
}


void dataBuffer2::resize( const size2& size, const FloatType initValue )
{
    dataBuffer2 tempBuffer;
    tempBuffer.mData.resize( (SizeType)size.x * size.y , initValue );
    tempBuffer.mSize = size;
    SizeType iMin = (SizeType)std::min(size.x, mSize.x);
    SizeType jMin = (SizeType)std::min(size.y, mSize.y);
   
    for (SizeType j = 0; j < jMin; ++j) 
    {
        for (SizeType i = 0; i < iMin; ++i) 
        {
            tempBuffer(i, j ) = valueByIndex(i, j );
        }
    }

    swap(tempBuffer);
}

void dataBuffer2::resize( SizeType width, SizeType height, const FloatType initValue )
{
    resize( size2(width, height), initValue );
}

const FloatType dataBuffer2::valueByIndex( SizeType i, SizeType j ) const
{
    return mData[i + mSize.x * j];
}

FloatType& dataBuffer2::valueByIndex( SizeType i, SizeType j )
{
    return mData[i + mSize.x * j];
}


void dataBuffer2::set( FloatType value )
{
    for ( FloatType &v: mData )
    {
        v = value;
    }
}

void dataBuffer2::set( const dataBuffer2& other )
{
    mData.resize(other.mData.size());
    std::copy(other.mData.begin(), other.mData.end(), mData.begin());
    mSize = other.mSize;
}

size2 dataBuffer2::size( ) const
{
    return mSize;
}

SizeType dataBuffer2::width( ) const
{
    return mSize.x;
}

SizeType dataBuffer2::height( ) const
{
    return mSize.y;
}



FloatType *dataBuffer2::data( )
{
    return mData.data();
}

std::vector<FloatType>::iterator dataBuffer2::begin()
{
    return mData.begin();
}

std::vector<FloatType>::const_iterator dataBuffer2::begin() const
{
    return mData.begin();
}

std::vector<FloatType>::iterator dataBuffer2::end()
{
    return mData.end();
}

std::vector<FloatType>::const_iterator dataBuffer2::end() const
{
    return mData.end();
}

void dataBuffer2::swap( dataBuffer2& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}


FloatType& dataBuffer2::operator()(SizeType i, SizeType j)
{
    return mData[i + mSize.x * j];
}

const FloatType& dataBuffer2::operator()( SizeType i, SizeType j ) const
{
    return mData[i + mSize.x * j];
}


