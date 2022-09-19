//---timer.cpp

#include <iostream>
#include "timer.h"

timer::timer( const std::string &info )
{
    mInfo = info;
    mStartingPoint = mClock.now();
}

timer::~timer( )
{
    std::cout << mInfo << " took " << durationInSeconds() << " seconds" << std::endl;
}

FloatType timer::durationInSeconds() const 
{
    auto end = std::chrono::steady_clock::now();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(
        end - mStartingPoint).count();
    return count / (FloatType)1000000.0;
}

void timer::reset() {
    mStartingPoint = mClock.now();
}

