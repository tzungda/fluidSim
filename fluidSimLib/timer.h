//---timer.h
#ifndef timer_H
#define timer_H

#include <chrono>
#include <string>

#include "common.h"

class timer {
public:
    //! Constructs the timer and start ticking.
    timer();
    timer( const std::string &info );
    virtual ~timer();

    //! Returns the time duration since the creation or reset in seconds.
    FloatType durationInSeconds() const;

    //! Resets the timer.
    void reset();

private:
    std::string mInfo;
    std::chrono::steady_clock mClock;
    std::chrono::steady_clock::time_point mStartingPoint;
};

#endif


