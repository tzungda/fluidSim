//---dataBuffer3.h
#ifndef dataBuffer3_H
#define dataBuffer3_H

#include <vector>
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

    dataBuffer3( SizeType width, SizeType height, SizeType depth, const FloatType initValue = 0.0 );

public:
    void resize( const size3& size, const FloatType initValue = 0.0 );

    void resize( SizeType width, SizeType height, SizeType depth, const FloatType initValue = 0.0 );

    const FloatType valueByIndex( SizeType i, SizeType j, SizeType k ) const;

    FloatType& valueByIndex( SizeType i, SizeType j, SizeType k );

    template <typename Callback>
    void forEachIndex(Callback func) const
    {
        for (SizeType k = 0; k < mSize.z; ++k)
        {
            for (SizeType j = 0; j < mSize.y; ++j)
            {
                for (SizeType i = 0; i < mSize.x; ++i)
                {
                    func(i, j, k);
                }
            }
        }
    }

    template <typename Callback>
    void forEach(Callback func) const
    {
        for (SizeType k = 0; k < mSize.z; ++k)
        {
            for (SizeType j = 0; j < mSize.y; ++j)
            {
                for (SizeType i = 0; i < mSize.x; ++i)
                {
                    func((*this)(i, j, k));
                }
            }
        }
    }

    void set( FloatType value );
    void set( const dataBuffer3& value );

    size3 size( ) const;

    SizeType width( ) const;

    SizeType height( ) const;

    SizeType depth( ) const;

    FloatType *data( );

    std::vector<FloatType>::iterator begin();
    std::vector<FloatType>::const_iterator begin() const;
    std::vector<FloatType>::iterator end();
    std::vector<FloatType>::const_iterator end() const;

    void swap( dataBuffer3& other );

    FloatType& operator()(SizeType i, SizeType j, SizeType k);
    const FloatType& operator()( SizeType i, SizeType j, SizeType k ) const;
    //
    FloatType& operator()( const size3& pt );
    const FloatType& operator()( const size3& pt ) const;
};

#endif

