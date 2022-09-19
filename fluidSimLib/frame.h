//---frame.h
#ifndef frame_H
#define frame_H

#include "common.h"

struct frame
{
    frame()
    {
        index = 0;
        timeIntervalInSeconds = (FloatType)(1.0 / 60.0);
    }

    frame(unsigned int newIndex, FloatType newTimeInvervalInSeconds)
    {
        index = newIndex;
        timeIntervalInSeconds = newTimeInvervalInSeconds;
    }

    FloatType timeInSeconds() const
    {
        return timeIntervalInSeconds * (FloatType)index;
    }

    void advance()
    {
        ++index;
    }

    void advance(unsigned int delta)
    {
        index += delta;
    }

    frame& operator++()
    {
        advance();
        return *this;
    }


    unsigned int index;
    FloatType timeIntervalInSeconds;
};

#endif

