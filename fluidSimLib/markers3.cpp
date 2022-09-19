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

markers3::markers3( SizeType width, SizeType height, SizeType depth, const char initValue )
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

void markers3::resize( const size3& size, const char initValue )
{
    markers3 tempBuffer;
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

void markers3::resize( SizeType width, SizeType height, SizeType depth, const char initValue )
{
    resize( size3(width, height, depth), initValue );
}

const char markers3::valueByIndex( SizeType i, SizeType j, SizeType k ) const
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}

char& markers3::valueByIndex( SizeType i, SizeType j, SizeType k )
{
    return mData[i + mSize.x * ( j + k * mSize.z ) ];
}


size3 markers3::size( ) const
{
    return mSize;
}

SizeType markers3::width( ) const
{
    return mSize.x;
}

SizeType markers3::height( ) const
{
    return mSize.y;
}

SizeType markers3::depth( ) const
{
    return mSize.z;
}

char *markers3::data( )
{
    return mData.data();
}

void markers3::swap( markers3& other )
{
    std::swap(other.mData, mData);
    std::swap(other.mSize, mSize);
}


char& markers3::operator()(SizeType i, SizeType j, SizeType k)
{
    return mData[i + mSize.x * (j + mSize.y * k)];
}

const char& markers3::operator()(SizeType i, SizeType j, SizeType k) const
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

