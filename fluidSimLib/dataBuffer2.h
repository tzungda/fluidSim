//---dataBuffer2.h
#ifndef dataBuffer2_H
#define dataBuffer2_H

#include <vector>
#include "size2.h"

class dataBuffer2
{
private:
    size2 mSize;
    std::vector<double> mData;

public:
    dataBuffer2();

    dataBuffer2( const size2& size, const double initValue = 0.0 );

    dataBuffer2( size_t width, size_t height, const double initValue = 0.0 );

public:
    void resize( const size2& size, const double initValue = 0.0 );

    void resize( size_t width, size_t height, const double initValue = 0.0 );

    const double valueByIndex( size_t i, size_t j ) const;

    double& valueByIndex( size_t i, size_t j );

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

    void set( double value );
    void set( const dataBuffer2& value );

    size2 size( ) const;

    size_t width( ) const;

    size_t height( ) const;

    double *data( );

    std::vector<double>::iterator begin();
    std::vector<double>::const_iterator begin() const;
    std::vector<double>::iterator end();
    std::vector<double>::const_iterator end() const;

    void swap( dataBuffer2& other );

    double& operator()(size_t i, size_t j);
    const double& operator()( size_t i, size_t j ) const;
};

#endif


