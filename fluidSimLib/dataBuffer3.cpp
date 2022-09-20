//---dataBuffer3.cpp

#include <algorithm>
#include "dataBuffer3.h"

dataBuffer3::dataBuffer3()
{
}

dataBuffer3::dataBuffer3( const size3& size, const double initValue )
{
    resize(size, initValue);
}

dataBuffer3::dataBuffer3( size_t width, size_t height, size_t depth, const double initValue )
{
    resize(width, height, depth, initValue);
}


void dataBuffer3::resize( const size3& size, const double initValue )
{
    dataBuffer3 tempBuffer;
    tempBuffer.mData.resize( (size_t)size.x * size.y * size.z, initValue );
    tempBuffer.mSize = size;
    size_t iMin = (size_t)std::min(size.x, mSize.x);
    size_t jMin = (size_t)std::min(size.y, mSize.y);
    size_t kMin = (size_t)std::min(size.z, mSize.z);
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

void dataBuffer3::resize( size_t width, size_t height, size_t depth, const double initValue )
{
    resize( size3(width, height, depth), initValue );
}

const double dataBuffer3::valueByIndex( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

double& dataBuffer3::valueByIndex( size_t i, size_t j, size_t k )
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

void dataBuffer3::set( double value )
{
    for ( double &v: mData )
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

void dataBuffer3::setZero()
{
    memset( &mData[0], 0, sizeof( double )*mData.size() );
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

double *dataBuffer3::data( )
{
    return mData.data();
}

std::vector<double>::iterator dataBuffer3::begin()
{
    return mData.begin();
}

std::vector<double>::const_iterator dataBuffer3::begin() const
{
    return mData.begin();
}

std::vector<double>::iterator dataBuffer3::end()
{
    return mData.end();
}

std::vector<double>::const_iterator dataBuffer3::end() const
{
    return mData.end();
}

void dataBuffer3::swap( dataBuffer3& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}


double& dataBuffer3::operator()(size_t i, size_t j, size_t k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const double& dataBuffer3::operator()( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

double& dataBuffer3::operator()( const size3& pt )
{
    return mData[pt.x + mSize.x * (pt.y + mSize.y * pt.z)];
}

const double& dataBuffer3::operator()( const size3& pt ) const
{
    return mData[pt.x + mSize.x * (pt.y + mSize.y * pt.z)];
}
