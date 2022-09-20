//---markers3.h
#ifndef markers3_H
#define markers3_H

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "size3.h"
#include "vector3.h"

class markers3
{
private:
    size3 mSize;
    std::vector<char> mData;

public:
    markers3( );

    markers3( const size3& size, const char initValue = 0.0 );

    markers3( size_t width, size_t height, size_t depth, const char initValue = 0 );

public:
    void set( char value );
    void resize( const size3& size, const char initValue = 0.0 );

    void resize( size_t width, size_t height, size_t depth, const char initValue = 0 );

    const char valueByIndex( size_t i, size_t j, size_t k ) const;

    char& valueByIndex( size_t i, size_t j, size_t k );

#ifdef _OPENMP
    template <typename Callback>
    void forEachIndexOpenMP(Callback func) const
    {
#pragma omp parallel for
        for ( int k = 0; k < (int)mSize.z; ++k)
        {
            for ( int j = 0; j < (int)mSize.y; ++j)
            {
                for ( int i = 0; i < (int)mSize.x; ++i)
                {
                    func(i, j, k);
                }
            }
        }
    }
#endif

    template <typename Callback>
    void forEachIndex(Callback func) const
    {
        for (size_t k = 0; k < mSize.z; ++k)
        {
            for (size_t j = 0; j < mSize.y; ++j)
            {
                for (size_t i = 0; i < mSize.x; ++i)
                {
                    func(i, j, k);
                }
            }
        }
    }

    size3 size( ) const;

    size_t width( ) const;

    size_t height( ) const;

    size_t depth( ) const;

    char *data( );

    void swap( markers3& other );

    char& operator()(size_t i, size_t j, size_t k);
    const char& operator()(size_t i, size_t j, size_t k) const;
    //
    char& operator()( const size3& pt );
    const char& operator()( const size3& pt ) const;

};

#endif

