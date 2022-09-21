//---gridEmitter3.cpp

#include "gridEmitter3.h"

gridEmitter3::gridEmitter3() 
{
}

gridEmitter3::~gridEmitter3() 
{
}

void gridEmitter3::update(
    FloatType currentTimeInSeconds,
    FloatType timeIntervalInSeconds) 
{
    if (mOnBeginUpdateCallback) 
    {
        mOnBeginUpdateCallback(
            this, currentTimeInSeconds, timeIntervalInSeconds);
    }

    onUpdate(currentTimeInSeconds, timeIntervalInSeconds);
}

void gridEmitter3::setOnBeginUpdateCallback(
    const OnBeginUpdateCallback& callback) 
{
    mOnBeginUpdateCallback = callback;
}

void gridEmitter3::callOnBeginUpdateCallback(
    FloatType currentTimeInSeconds,
    FloatType timeIntervalInSeconds) 
{
    mOnBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
}



