//---dataBuffer3.h
#ifndef dataBuffer3_H
#define dataBuffer3_H

#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "size3.h"
#include "vector3.h"

class dataBuffer3
{
private:
    size3 mSize;
    std::vector<FloatType> mData;

public:
    dataBuffer3();

    dataBuffer3( const size3& size, const FloatType initValue = 0.0 );

    dataBuffer3( size_t width, size_t height, size_t depth, const FloatType initValue = 0.0 );

public:
    void resize( const size3& size, const FloatType initValue = 0.0 );

    void resize( size_t width, size_t height, size_t depth, const FloatType initValue = 0.0 );

    const FloatType valueByIndex( size_t i, size_t j, size_t k ) const;

    FloatType& valueByIndex( size_t i, size_t j, size_t k );

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

#ifdef _OPENMP
    template <typename Callback>
    void forEachIndexOpenMP(Callback func) const
    {
#pragma omp parallel for
        for (int k = 0; k < (int)mSize.z; ++k)
        {
            for (int j = 0; j < (int)mSize.y; ++j)
            {
                for (int i = 0; i < (int)mSize.x; ++i)
                {
                    func(i, j, k);
                }
            }
        }
    }
#endif

    template <typename Callback>
    void forEach(Callback func) const
    {
        for (size_t k = 0; k < mSize.z; ++k)
        {
            for (size_t j = 0; j < mSize.y; ++j)
            {
                for (size_t i = 0; i < mSize.x; ++i)
                {
                    func((*this)(i, j, k));
                }
            }
        }
    }

#ifdef _OPENMP
    template <typename Callback>
    void forEachOpenMP(Callback func) const
    {
#pragma omp parallel for
        for (int k = 0; k < (int)mSize.z; ++k)
        {
            for (int j = 0; j < (int)mSize.y; ++j)
            {
                for (int i = 0; i < (int)mSize.x; ++i)
                {
                    func((*this)(i, j, k));
                }
            }
        }

    }
#endif

    void set( FloatType value );
    void set( const dataBuffer3& value );
    void setZero();

    size3 size( ) const;

    size_t width( ) const;

    size_t height( ) const;

    size_t depth( ) const;

    FloatType *data( );

    std::vector<FloatType>::iterator begin();
    std::vector<FloatType>::const_iterator begin() const;
    std::vector<FloatType>::iterator end();
    std::vector<FloatType>::const_iterator end() const;

    void swap( dataBuffer3& other );

    FloatType& operator()(size_t i, size_t j, size_t k);
    const FloatType& operator()( size_t i, size_t j, size_t k ) const;
    //
    FloatType& operator()( const size3& pt );
    const FloatType& operator()( const size3& pt ) const;
};

#endif

