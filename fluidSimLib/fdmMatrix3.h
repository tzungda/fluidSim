//---fdmMatrix3.h
#ifndef fdmMatrix3_H
#define fdmMatrix3_H

#include <vector>

#include "size3.h"

struct fdmMatrixRow3 {
    //! Diagonal component of the matrix (row, row).
    FloatType center = 0.0;
    //! Off-diagonal element where colum refers to (i+1, j, k) grid point.
    FloatType right = 0.0;
    //! Off-diagonal element where column refers to (i, j+1, k) grid point.
    FloatType up = 0.0;
    //! OFf-diagonal element where column refers to (i, j, k+1) grid point.
    FloatType front = 0.0;
};

class fdmMatrix3
{
private:
    size3 mSize;
    std::vector<fdmMatrixRow3> mData;

public:
    fdmMatrix3();

    fdmMatrix3( const size3& size, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    fdmMatrix3( SizeType width, SizeType height, SizeType depth, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    fdmMatrix3( const fdmMatrix3& other );

public:
    void resize( const size3& size, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    void resize( SizeType width, SizeType height, SizeType depth, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    const fdmMatrixRow3& valueByIndex( SizeType i, SizeType j, SizeType k ) const;

    fdmMatrixRow3& valueByIndex( SizeType i, SizeType j, SizeType k );

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

    void set( const fdmMatrixRow3& value );
    void set( const fdmMatrix3& value );

    size3 size( ) const;

    SizeType width( ) const;

    SizeType height( ) const;

    SizeType depth( ) const;

    fdmMatrixRow3 *data( );

    void swap( fdmMatrix3& other );

    fdmMatrixRow3& operator()(SizeType i, SizeType j, SizeType k);
    const fdmMatrixRow3& operator()( SizeType i, SizeType j, SizeType k ) const;
};

#endif

