//---simAnimation.h
#ifndef simAnimation_H
#define simAnimation_H

#include "animFrame.h"

class simAnimation
{
public:

    simAnimation();

    virtual ~simAnimation();

    void update( const animFrame& f )
    {
        onUpdate( f );
    }

    bool isUsingFixedSubTimeSteps() const;

    void setIsUsingFixedSubTimeSteps(bool isUsing);

    unsigned int numberOfFixedSubTimeSteps() const;

    void setNumberOfFixedSubTimeSteps(unsigned int numberOfSteps);

    virtual unsigned int numberOfSubTimeSteps(
        FloatType timeIntervalInSeconds) const;

    //! Advances a single animFrame.
    void advanceSingleFrame();

    animFrame currentFrame() const;

    void setCurrentFrame(const animFrame& animFrame);

    FloatType currentTimeInSeconds() const;


    virtual void onAdvanceTimeStep(FloatType timeIntervalInSeconds) = 0;

    virtual void onInitialize();

private:
    animFrame mCurrentFrame;
    bool mIsUsingFixedSubTimeSteps = true;
    unsigned int mNumberOfFixedSubTimeSteps = 1;
    bool mHasInitialized = false;
    FloatType mCurrentTime = 0.0;

    void onUpdate(const animFrame& f);
    void advanceTimeStep( FloatType timeIntervalInSeconds );
    void initialize();
};

#endif
