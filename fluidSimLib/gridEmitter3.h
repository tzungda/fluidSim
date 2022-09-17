//---gridEmitter3.h
#ifndef gridEmitter3_H
#define gridEmitter3_H

#include <memory>
#include <vector>
#include <utility>
#include <functional>

class gridEmitter3
{
public:
    typedef std::function<void(gridEmitter3*, double, double)>
        OnBeginUpdateCallback;

    gridEmitter3();

    virtual ~gridEmitter3();

    void update(double currentTimeInSeconds, double timeIntervalInSeconds);

    void setOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

protected:
    virtual void onUpdate(
        double currentTimeInSeconds,
        double timeIntervalInSeconds) = 0;

    void callOnBeginUpdateCallback(
        double currentTimeInSeconds,
        double timeIntervalInSeconds);

private:
    OnBeginUpdateCallback mOnBeginUpdateCallback;
};

typedef std::shared_ptr<gridEmitter3> gridEmitter3Ptr;

#endif

