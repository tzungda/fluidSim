//---size2.cpp

#include "size2.h"

size2::size2()
{
    x = y = 0;
}

size2::size2(SizeType _x, SizeType _y)
{
    x = _x, y = _y;
}


size2& size2::operator=(const size2& v)
{
    x = v.x;
    y = v.y;

    return (*this);
}

bool size2::operator!=(const size2& v) const
{
    if (x != v.x)
        return false;
    if (y != v.y)
        return false;

    return true;
}


size2 operator+(const size2& a, const size2& b)
{
    return size2(a.x + b.x, a.y + b.y);
}



