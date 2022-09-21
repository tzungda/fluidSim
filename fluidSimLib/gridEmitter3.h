//---gridEmitter3.h
#ifndef gridEmitter3_H
#define gridEmitter3_H

#include <memory>
#include <vector>
#include <utility>
#include <functional>

#include "common.h"

class gridEmitter3
{
public:
    typedef std::function<void(gridEmitter3*, FloatType, FloatType)>
        OnBeginUpdateCallback;

    gridEmitter3();

    virtual ~gridEmitter3();

    void update(FloatType currentTimeInSeconds, FloatType timeIntervalInSeconds);

    void setOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

protected:
    virtual void onUpdate(
        FloatType currentTimeInSeconds,
        FloatType timeIntervalInSeconds) = 0;

    void callOnBeginUpdateCallback(
        FloatType currentTimeInSeconds,
        FloatType timeIntervalInSeconds);

private:
    OnBeginUpdateCallback mOnBeginUpdateCallback;
};

typedef std::shared_ptr<gridEmitter3> gridEmitter3Ptr;

#endif

