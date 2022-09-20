//---fdmMatrix3.h
#ifndef fdmMatrix3_H
#define fdmMatrix3_H

#include <vector>

#include "size3.h"

struct fdmMatrixRow3 {
    //! Diagonal component of the matrix (row, row).
    double center = 0.0;
    //! Off-diagonal element where colum refers to (i+1, j, k) grid point.
    double right = 0.0;
    //! Off-diagonal element where column refers to (i, j+1, k) grid point.
    double up = 0.0;
    //! OFf-diagonal element where column refers to (i, j, k+1) grid point.
    double front = 0.0;
};

class fdmMatrix3
{
private:
    size3 mSize;
    std::vector<fdmMatrixRow3> mData;

public:
    fdmMatrix3();

    fdmMatrix3( const size3& size, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    fdmMatrix3( size_t width, size_t height, size_t depth, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    fdmMatrix3( const fdmMatrix3& other );

public:
    void resize( const size3& size, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    void resize( size_t width, size_t height, size_t depth, const fdmMatrixRow3 &initValue = fdmMatrixRow3() );

    const fdmMatrixRow3& valueByIndex( size_t i, size_t j, size_t k ) const;

    fdmMatrixRow3& valueByIndex( size_t i, size_t j, size_t k );

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

    void set( const fdmMatrixRow3& value );
    void set( const fdmMatrix3& value );

    size3 size( ) const;

    size_t width( ) const;

    size_t height( ) const;

    size_t depth( ) const;

    fdmMatrixRow3 *data( );

    void swap( fdmMatrix3& other );

    fdmMatrixRow3& operator()(size_t i, size_t j, size_t k);
    const fdmMatrixRow3& operator()( size_t i, size_t j, size_t k ) const;
};

#endif

