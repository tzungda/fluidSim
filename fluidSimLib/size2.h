//---size2.h
#ifndef size2_H
#define size2_H

class size2
{
public:

    size_t x;

    size_t y;


    size2();

    size2( size_t _x, size_t _y );

    size2& operator=(const size2& v);
    bool operator!=(const size2& v) const;
};

size2 operator+(const size2& a, const size2& b);

#endif

