//---simAnimation.cpp

#include <limits>
#include "simAnimation.h"

simAnimation::simAnimation()
{
    mIsUsingFixedSubTimeSteps = true;
    mNumberOfFixedSubTimeSteps = 1;
    mHasInitialized = false;
    mCurrentTime = 0.0;
}


simAnimation::~simAnimation()
{
}


void simAnimation::onUpdate(const animFrame& f)
{
    if (f.index > mCurrentFrame.index)
    {
        unsigned int numberOfFrames = f.index - mCurrentFrame.index;

        for (unsigned int i = 0; i < numberOfFrames; ++i)
        {
            advanceTimeStep(f.timeIntervalInSeconds);
        }

        mCurrentFrame = f;
    }
    else if (f.index == 0 && !mHasInitialized)
    {
        initialize();
    }
}


void simAnimation::advanceTimeStep( FloatType timeIntervalInSeconds )
{
    mCurrentTime = mCurrentFrame.timeInSeconds();

    if ( mIsUsingFixedSubTimeSteps )
    {
        // perform fixed time-stepping
        const FloatType actualTimeInterval
            = timeIntervalInSeconds / static_cast<FloatType>(mNumberOfFixedSubTimeSteps);

        for (unsigned int i = 0; i < mNumberOfFixedSubTimeSteps; ++i)
        {
            onAdvanceTimeStep(actualTimeInterval);

            mCurrentTime += actualTimeInterval;
        }
    }
    else
    {
        // perform adaptive time-stepping
        FloatType remainingTime = timeIntervalInSeconds;
        while (remainingTime > std::numeric_limits<FloatType>::epsilon())
        {
            unsigned int numSteps = numberOfSubTimeSteps(remainingTime);
            FloatType actualTimeInterval
                = remainingTime / static_cast<FloatType>(numSteps);

            onAdvanceTimeStep(actualTimeInterval);
            remainingTime -= actualTimeInterval;
            mCurrentTime += actualTimeInterval;
        }
    }
}

bool simAnimation::isUsingFixedSubTimeSteps() const
{
    return mIsUsingFixedSubTimeSteps;
}

void simAnimation::setIsUsingFixedSubTimeSteps(bool isUsing)
{
    mIsUsingFixedSubTimeSteps = isUsing;
}

unsigned int simAnimation::numberOfFixedSubTimeSteps() const
{
    return mNumberOfFixedSubTimeSteps;
}

void simAnimation::setNumberOfFixedSubTimeSteps(unsigned int numberOfSteps)
{
    mNumberOfFixedSubTimeSteps = numberOfSteps;
}

unsigned int simAnimation::numberOfSubTimeSteps(
    FloatType timeIntervalInSeconds) const
{
    return mNumberOfFixedSubTimeSteps;
}

void simAnimation::advanceSingleFrame()
{
    ++mCurrentFrame;
    update(mCurrentFrame);
}

animFrame simAnimation::currentFrame() const
{
    return mCurrentFrame;
}

void simAnimation::setCurrentFrame(const animFrame& animFrame)
{
    mCurrentFrame = animFrame;
}

FloatType simAnimation::currentTimeInSeconds() const
{
    return mCurrentTime;
}

void simAnimation::initialize()
{
    onInitialize();
    mHasInitialized = true;
}

void simAnimation::onInitialize()
{
}