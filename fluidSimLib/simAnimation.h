//---simAnimation.h
#ifndef simAnimation_H
#define simAnimation_H

#include "frame.h"
#include "common.h"

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

    //! Advances a single frame.
    void advanceSingleFrame();

    frame currentFrame() const;

    void setCurrentFrame(const frame& frame);

    FloatType currentTimeInSeconds() const;
   

    virtual void onAdvanceTimeStep(FloatType timeIntervalInSeconds) = 0;
   
    virtual void onInitialize();

protected:
    frame mCurrentFrame;
private:
    
    bool mIsUsingFixedSubTimeSteps = true;
    unsigned int mNumberOfFixedSubTimeSteps = 1;
    bool mHasInitialized = false;
    FloatType mCurrentTime = 0.0;

    void onUpdate(const frame& f);
    void advanceTimeStep( FloatType timeIntervalInSeconds );
    void initialize();
};

#endif


