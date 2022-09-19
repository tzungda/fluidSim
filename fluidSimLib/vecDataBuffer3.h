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

    vecDataBuffer3( SizeType width, SizeType height, SizeType depth, const vector3 initValue = vector3() );

public:
    void resize( const size3& size, const vector3 initValue = vector3() );

    void resize( SizeType width, SizeType height, SizeType depth, const vector3 initValue = vector3() );

    const vector3 valueByIndex( SizeType i, SizeType j, SizeType k ) const;

    vector3& valueByIndex( SizeType i, SizeType j, SizeType k );

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
                    func( (*this)(i, j, k) );
                }
            }
        }
    }

    size3 size( ) const;

    SizeType width( ) const;

    SizeType height( ) const;

    SizeType depth( ) const;

    vector3 *data( );

    void swap( vecDataBuffer3& other );

    void set( const vecDataBuffer3& other );

    vector3& operator()(SizeType i, SizeType j, SizeType k);
    const vector3& operator()(SizeType i, SizeType j, SizeType k) const;
};

#endif

