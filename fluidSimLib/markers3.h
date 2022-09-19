//---markers3.h
#ifndef markers3_H
#define markers3_H

#include <vector>
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

    markers3( SizeType width, SizeType height, SizeType depth, const char initValue = 0 );

public:
    void set( char value );
    void resize( const size3& size, const char initValue = 0.0 );

    void resize( SizeType width, SizeType height, SizeType depth, const char initValue = 0 );

    const char valueByIndex( SizeType i, SizeType j, SizeType k ) const;

    char& valueByIndex( SizeType i, SizeType j, SizeType k );

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

    size3 size( ) const;

    SizeType width( ) const;

    SizeType height( ) const;

    SizeType depth( ) const;

    char *data( );

    void swap( markers3& other );

    char& operator()(SizeType i, SizeType j, SizeType k);
    const char& operator()(SizeType i, SizeType j, SizeType k) const;
    //
    char& operator()( const size3& pt );
    const char& operator()( const size3& pt ) const;

};

#endif

