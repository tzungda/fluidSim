//---size3.cpp

#include "size3.h"

size3::size3()
{
    x = y = z = 0;
}

size3::size3(size_t _x, size_t _y, size_t _z)
{
    x = _x, y = _y, z = _z;
}


size3& size3::operator=(const size3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return (*this);
}

bool size3::operator!=(const size3& v) const
{
    if (x != v.x)
        return false;
    if (y != v.y)
        return false;
    if (z != v.z)
        return false;
    return true;
}


size3 operator+(const size3& a, const size3& b)
{
    return size3(a.x + b.x, a.y + b.y, a.z + b.z);
}



