//---size2.h
#ifndef size2_H
#define size2_H

#include "common.h"

class size2
{
public:

    SizeType x;

    SizeType y;


    size2();

    size2( SizeType _x, SizeType _y );

    size2& operator=(const size2& v);
    bool operator!=(const size2& v) const;
};

size2 operator+(const size2& a, const size2& b);

#endif

