//---size2.cpp

#include "size2.h"

size2::size2()
{
    x = y = 0;
}

size2::size2(size_t _x, size_t _y)
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
        return true;
    if (y != v.y)
        return true;

    return false;
}


size2 operator+(const size2& a, const size2& b)
{
    return size2(a.x + b.x, a.y + b.y);
}



