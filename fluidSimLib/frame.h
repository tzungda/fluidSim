//---frame.h
#ifndef frame_H
#define frame_H

struct frame
{
    frame()
    {
        index = 0;
        timeIntervalInSeconds = 1.0 / 60.0;
    }

    frame(unsigned int newIndex, double newTimeInvervalInSeconds)
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

    frame& operator++()
    {
        advance();
        return *this;
    }


    unsigned int index;
    double timeIntervalInSeconds;
};

#endif

