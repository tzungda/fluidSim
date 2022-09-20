//---timer.h
#ifndef timer_H
#define timer_H

#include <chrono>

class timer {
public:
    //! Constructs the timer and start ticking.
    timer();

    //! Returns the time duration since the creation or reset in seconds.
    double durationInSeconds() const;

    //! Resets the timer.
    void reset();

private:
    std::chrono::steady_clock mClock;
    std::chrono::steady_clock::time_point mStartingPoint;
};

#endif


