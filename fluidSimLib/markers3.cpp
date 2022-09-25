//---markers3.cpp

#include <algorithm>
#include "markers3.h"

markers3::markers3()
{
}

markers3::markers3( const size3& size, const char initValue )
{
    resize(size, initValue);
}

markers3::markers3( size_t width, size_t height, size_t depth, const char initValue )
{
    resize(width, height, depth, initValue);
}

void markers3::set( char value )
{
    for ( char &v : mData )
    {
        v = value;
    }
}

void markers3::setZero()
{
    memset( &mData[0], 0, sizeof( char )*mData.size() );
}

void markers3::resize( const size3& size, const char initValue )
{
    markers3 tempBuffer;
    tempBuffer.mData.resize(size.x * size.y * size.z, initValue );
    tempBuffer.mSize = size;

    if ( mSize != size )
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
            memcpy( &tempBuffer.mData[0], &mData[0], sizeof( char )*mData.size() );
    }

    swap(tempBuffer);
}

void markers3::resize( size_t width, size_t height, size_t depth, const char initValue )
{
    resize( size3(width, height, depth), initValue );
}

const char markers3::valueByIndex( size_t i, size_t j, size_t k ) const
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

char& markers3::valueByIndex( size_t i, size_t j, size_t k )
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}


size3 markers3::size( ) const
{
    return mSize;
}

size_t markers3::width( ) const
{
    return mSize.x;
}

size_t markers3::height( ) const
{
    return mSize.y;
}

size_t markers3::depth( ) const
{
    return mSize.z;
}

char *markers3::data( )
{
    return mData.data();
}

const char *markers3::data( ) const
{
    return mData.data();
}

void markers3::swap( markers3& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}


char& markers3::operator()(size_t i, size_t j, size_t k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const char& markers3::operator()(size_t i, size_t j, size_t k) const
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

char& markers3::operator()( const size3& pt )
{
    return mData[pt.x + mSize.x * (pt.y + mSize.y * pt.z)];
}

const char& markers3::operator()( const size3& pt ) const
{
    return mData[pt.x + mSize.x * (pt.y + mSize.y * pt.z)];
}

