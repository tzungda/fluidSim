//---animFrame.h
#ifndef animFrame_H
#define animFrame_H

#include "common.h"

struct animFrame
{
    animFrame()
    {
        index = 0;
        timeIntervalInSeconds = (FloatType)1.0 / (FloatType)60.0;
    }

    animFrame(unsigned int newIndex, FloatType newTimeInvervalInSeconds)
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

    animFrame& operator++()
    {
        advance();
        return *this;
    }


    unsigned int index;
    FloatType timeIntervalInSeconds;
};

#endif

