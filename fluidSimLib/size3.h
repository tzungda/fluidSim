//---size3.h
#ifndef size3_H
#define size3_H

#include "common.h"

class size3
{
public:

    SizeType x;

    SizeType y;

    SizeType z;


    size3();

    size3( SizeType _x, SizeType _y, SizeType _z );

    size3& operator=(const size3& v);
    bool operator!=(const size3& v) const;
};

size3 operator+(const size3& a, const size3& b);

#endif

