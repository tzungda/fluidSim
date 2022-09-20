//---simAnimation.h
#ifndef simAnimation_H
#define simAnimation_H

#include "frame.h"

class simAnimation
{
public:

    simAnimation();

    virtual ~simAnimation();

    void update( const frame& f )
    {
        onUpdate( f );
    }

    bool isUsingFixedSubTimeSteps() const;

    void setIsUsingFixedSubTimeSteps(bool isUsing);

    unsigned int numberOfFixedSubTimeSteps() const;

    void setNumberOfFixedSubTimeSteps(unsigned int numberOfSteps);

    virtual unsigned int numberOfSubTimeSteps(
        double timeIntervalInSeconds) const;

    //! Advances a single frame.
    void advanceSingleFrame();

    frame currentFrame() const;

    void setCurrentFrame(const frame& frame);

    double currentTimeInSeconds() const;


    virtual void onAdvanceTimeStep(double timeIntervalInSeconds) = 0;

    virtual void onInitialize();

private:
    frame mCurrentFrame;
    bool mIsUsingFixedSubTimeSteps = true;
    unsigned int mNumberOfFixedSubTimeSteps = 1;
    bool mHasInitialized = false;
    double mCurrentTime = 0.0;

    void onUpdate(const frame& f);
    void advanceTimeStep( double timeIntervalInSeconds );
    void initialize();
};

#endif
