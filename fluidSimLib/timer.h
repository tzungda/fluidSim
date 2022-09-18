//---timer.h
#ifndef timer_H
#define timer_H

#include <chrono>
#include <string>

class timer {
public:
    
    timer( const std::string &info );
    virtual ~timer( );

    double durationInSeconds() const;

    void reset();

private:
    std::string mInfo;
    std::chrono::steady_clock mClock;
    std::chrono::steady_clock::time_point mStartingPoint;
};

#endif


