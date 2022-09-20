//---size3.h
#ifndef size3_H
#define size3_H

class size3
{
public:

    size_t x;

    size_t y;

    size_t z;


    size3();

    size3( size_t _x, size_t _y, size_t _z );

    size3& operator=(const size3& v);
    bool operator!=(const size3& v) const;
};

size3 operator+(const size3& a, const size3& b);

#endif

