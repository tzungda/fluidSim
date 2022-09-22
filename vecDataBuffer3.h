//---vecDataBuffer3.h
#ifndef vecDataBuffer3_H
#define vecDataBuffer3_H

#include <vector>
#include "size3.h"
#include "vector3.h"

class vecDataBuffer3
{
private:
    size3 mSize;
    std::vector<vector3> mData;

public:
    vecDataBuffer3();

    vecDataBuffer3( const size3& size, const vector3 initValue = vector3() );

    vecDataBuffer3( size_t width, size_t height, size_t depth, const vector3 initValue = vector3() );

public:
    void resize( const size3& size, const vector3 initValue = vector3() );

    void resize( size_t width, size_t height, size_t depth, const vector3 initValue = vector3() );

    const vector3 valueByIndex( size_t i, size_t j, size_t k ) const;

    vector3& valueByIndex( size_t i, size_t j, size_t k );

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

    template <typename Callback>
    void forEach(Callback func) const
    {
        for (size_t k = 0; k < mSize.z; ++k) 
        {
            for (size_t j = 0; j < mSize.y; ++j) 
            {
                for (size_t i = 0; i < mSize.x; ++i) 
                {
                    func( (*this)(i, j, k) );
                }
            }
        }
    }

    size3 size( ) const;

    size_t width( ) const;

    size_t height( ) const;

    size_t depth( ) const;

    vector3 *data( );

    void swap( vecDataBuffer3& other );

    void set( const vecDataBuffer3& other );
    void set( const vector3& value );
    void setZero( );

    vector3& operator()(size_t i, size_t j, size_t k);
    const vector3& operator()(size_t i, size_t j, size_t k) const;
};

#endif

