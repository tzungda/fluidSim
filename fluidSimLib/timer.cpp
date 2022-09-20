//---timer.cpp

#include "timer.h"

timer::timer()
{
    mStartingPoint = mClock.now();
}

double timer::durationInSeconds() const 
{
    auto end = std::chrono::steady_clock::now();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(
        end - mStartingPoint).count();
    return count / 1000000.0;
}

void timer::reset() {
    mStartingPoint = mClock.now();
}

