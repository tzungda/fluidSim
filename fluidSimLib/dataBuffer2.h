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

    dataBuffer2( SizeType width, SizeType height, const FloatType initValue = 0.0 );

public:
    void resize( const size2& size, const FloatType initValue = 0.0 );

    void resize( SizeType width, SizeType height, const FloatType initValue = 0.0 );

    const FloatType valueByIndex( SizeType i, SizeType j ) const;

    FloatType& valueByIndex( SizeType i, SizeType j );

    template <typename Callback>
    void forEachIndex(Callback func) const
    {
        for (SizeType j = 0; j < mSize.y; ++j) 
        {
            for (SizeType i = 0; i < mSize.x; ++i) 
            {
                func(i, j);
            }
        }
    }

    template <typename Callback>
    void forEach(Callback func) const
    {
        for (SizeType j = 0; j < mSize.y; ++j) 
        {
            for (SizeType i = 0; i < mSize.x; ++i) 
            {
                func((*this)(i, j));
            }
        }
    }

    void set( FloatType value );
    void set( const dataBuffer2& value );

    size2 size( ) const;

    SizeType width( ) const;

    SizeType height( ) const;

    FloatType *data( );

    std::vector<FloatType>::iterator begin();
    std::vector<FloatType>::const_iterator begin() const;
    std::vector<FloatType>::iterator end();
    std::vector<FloatType>::const_iterator end() const;

    void swap( dataBuffer2& other );

    FloatType& operator()(SizeType i, SizeType j);
    const FloatType& operator()( SizeType i, SizeType j ) const;
};

#endif


