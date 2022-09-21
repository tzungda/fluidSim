//---animFrame.h
#ifndef animFrame_H
#define animFrame_H

struct animFrame
{
    animFrame()
    {
        index = 0;
        timeIntervalInSeconds = 1.0 / 60.0;
    }

    animFrame(unsigned int newIndex, double newTimeInvervalInSeconds)
    {
        index = newIndex;
        timeIntervalInSeconds = newTimeInvervalInSeconds;
    }

    double timeInSeconds() const
    {
        return timeIntervalInSeconds * (double)index;
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
    double timeIntervalInSeconds;
};

#endif

