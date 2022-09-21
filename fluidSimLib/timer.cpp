//---timer.cpp

#include <iostream>
#include "timer.h"

timer::timer()
{
    mStartingPoint = mClock.now();
}

timer::timer( const std::string &info )
{
    mStartingPoint = mClock.now();
    mInfo = info;
}

timer::~timer()
{
    std::cout << mInfo << " took " << durationInSeconds() << " seconds " << std::endl;
}

FloatType timer::durationInSeconds() const 
{
    auto end = std::chrono::steady_clock::now();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(
        end - mStartingPoint).count();
    return (FloatType)count / (FloatType)1000000.0;
}

void timer::reset() {
    mStartingPoint = mClock.now();
}

