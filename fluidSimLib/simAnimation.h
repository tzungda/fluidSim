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
        double timeIntervalInSeconds) const;

    //! Advances a single animFrame.
    void advanceSingleFrame();

    animFrame currentFrame() const;

    void setCurrentFrame(const animFrame& animFrame);

    double currentTimeInSeconds() const;


    virtual void onAdvanceTimeStep(double timeIntervalInSeconds) = 0;

    virtual void onInitialize();

private:
    animFrame mCurrentFrame;
    bool mIsUsingFixedSubTimeSteps = true;
    unsigned int mNumberOfFixedSubTimeSteps = 1;
    bool mHasInitialized = false;
    double mCurrentTime = 0.0;

    void onUpdate(const animFrame& f);
    void advanceTimeStep( double timeIntervalInSeconds );
    void initialize();
};

#endif
