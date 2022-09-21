//---dataBuffer2.h
#ifndef dataBuffer2_H
#define dataBuffer2_H

#include <vector>
#include "size2.h"
#include "common.h"

class dataBuffer2
{
private:
    size2 mSize;
    std::vector<FloatType> mData;

public:
    dataBuffer2();

    dataBuffer2( const size2& size, const FloatType initValue = 0.0 );

    dataBuffer2( size_t width, size_t height, const FloatType initValue = 0.0 );

public:
    void resize( const size2& size, const FloatType initValue = 0.0 );

    void resize( size_t width, size_t height, const FloatType initValue = 0.0 );

    const FloatType valueByIndex( size_t i, size_t j ) const;

    FloatType& valueByIndex( size_t i, size_t j );

    template <typename Callback>
    void forEachIndex(Callback func) const
    {
        for (size_t j = 0; j < mSize.y; ++j) 
        {
            for (size_t i = 0; i < mSize.x; ++i) 
            {
                func(i, j);
            }
        }
    }

    template <typename Callback>
    void forEach(Callback func) const
    {
        for (size_t j = 0; j < mSize.y; ++j) 
        {
            for (size_t i = 0; i < mSize.x; ++i) 
            {
                func((*this)(i, j));
            }
        }
    }

    void set( FloatType value );
    void set( const dataBuffer2& value );

    size2 size( ) const;

    size_t width( ) const;

    size_t height( ) const;

    FloatType *data( );

    std::vector<FloatType>::iterator begin();
    std::vector<FloatType>::const_iterator begin() const;
    std::vector<FloatType>::iterator end();
    std::vector<FloatType>::const_iterator end() const;

    void swap( dataBuffer2& other );

    FloatType& operator()(size_t i, size_t j);
    const FloatType& operator()( size_t i, size_t j ) const;
};

#endif


