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

dataBuffer3::dataBuffer3( size_t width, size_t height, size_t depth, const FloatType initValue )
{
    resize(width, height, depth, initValue);
}


void dataBuffer3::resize( const size3& size, const FloatType initValue )
{
    dataBuffer3 tempBuffer;
    tempBuffer.mData.resize( (size_t)size.x * size.y * size.z, initValue );
    tempBuffer.mSize = size;

    if ( mSize != size )
    {
        size_t iMin = (size_t)std::min(size.x, mSize.x);
        size_t jMin = (size_t)std::min(size.y, mSize.y);
        size_t kMin = (size_t)std::min(size.z, mSize.z);
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
            memcpy( &tempBuffer.mData[0], &mData[0], sizeof( FloatType )*mData.size() );
    }

    swap(tempBuffer);
}

void dataBuffer3::resize( size_t width, size_t height, size_t depth, const FloatType initValue )
{
    resize( size3(width, height, depth), initValue );
}

const FloatType dataBuffer3::valueByIndex( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

FloatType& dataBuffer3::valueByIndex( size_t i, size_t j, size_t k )
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

void dataBuffer3::set( FloatType value )
{
    std::fill( mData.begin(), mData.end(), value );
}

void dataBuffer3::set( const dataBuffer3& other )
{
    mData.resize(other.mData.size());
    std::copy(other.mData.begin(), other.mData.end(), mData.begin());
    mSize = other.mSize;
}

void dataBuffer3::setZero()
{
    memset( &mData[0], 0, sizeof( FloatType )*mData.size() );
}

size3 dataBuffer3::size( ) const
{
    return mSize;
}

size_t dataBuffer3::width( ) const
{
    return mSize.x;
}

size_t dataBuffer3::height( ) const
{
    return mSize.y;
}

size_t dataBuffer3::depth( ) const
{
    return mSize.z;
}

FloatType *dataBuffer3::data( )
{
    return mData.data();
}

const FloatType *dataBuffer3::data( ) const
{
    return mData.data();
}

size_t dataBuffer3::dataLength( ) const
{
    return mData.size();
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


const FloatType* dataBuffer3::iBuffer( size_t j, size_t k, size_t i) const
{
    return &mData[ i + mSize.x * (j + mSize.y * k)];
}

FloatType* dataBuffer3::iBuffer( size_t j, size_t k, size_t i ) 
{
    return &mData[i + mSize.x * (j + mSize.y * k)];
}


FloatType& dataBuffer3::operator()(size_t i, size_t j, size_t k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const FloatType& dataBuffer3::operator()( size_t i, size_t j, size_t k ) const
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
