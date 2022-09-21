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

dataBuffer2::dataBuffer2( size_t width, size_t height, const FloatType initValue )
{
    resize(width, height, initValue);
}


void dataBuffer2::resize( const size2& size, const FloatType initValue )
{
    dataBuffer2 tempBuffer;
    tempBuffer.mData.resize( (size_t)size.x * size.y , initValue );
    tempBuffer.mSize = size;
    size_t iMin = (size_t)std::min(size.x, mSize.x);
    size_t jMin = (size_t)std::min(size.y, mSize.y);
   
    for (size_t j = 0; j < jMin; ++j) 
    {
        for (size_t i = 0; i < iMin; ++i) 
        {
            tempBuffer(i, j ) = valueByIndex(i, j );
        }
    }

    swap(tempBuffer);
}

void dataBuffer2::resize( size_t width, size_t height, const FloatType initValue )
{
    resize( size2(width, height), initValue );
}

const FloatType dataBuffer2::valueByIndex( size_t i, size_t j ) const
{
    return mData[i + mSize.x * j];
}

FloatType& dataBuffer2::valueByIndex( size_t i, size_t j )
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

size_t dataBuffer2::width( ) const
{
    return mSize.x;
}

size_t dataBuffer2::height( ) const
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


FloatType& dataBuffer2::operator()(size_t i, size_t j)
{
    return mData[i + mSize.x * j];
}

const FloatType& dataBuffer2::operator()( size_t i, size_t j ) const
{
    return mData[i + mSize.x * j];
}


