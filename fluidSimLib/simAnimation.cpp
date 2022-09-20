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


void simAnimation::onUpdate(const frame& f)
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


void simAnimation::advanceTimeStep( double timeIntervalInSeconds )
{
    mCurrentTime = mCurrentFrame.timeInSeconds();

    if ( mIsUsingFixedSubTimeSteps )
    {
        // perform fixed time-stepping
        const double actualTimeInterval
            = timeIntervalInSeconds / static_cast<double>(mNumberOfFixedSubTimeSteps);

        for (unsigned int i = 0; i < mNumberOfFixedSubTimeSteps; ++i)
        {
            onAdvanceTimeStep(actualTimeInterval);

            mCurrentTime += actualTimeInterval;
        }
    }
    else
    {
        // perform adaptive time-stepping
        double remainingTime = timeIntervalInSeconds;
        while (remainingTime > std::numeric_limits<double>::epsilon())
        {
            unsigned int numSteps = numberOfSubTimeSteps(remainingTime);
            double actualTimeInterval
                = remainingTime / static_cast<double>(numSteps);

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
    double timeIntervalInSeconds) const
{
    return mNumberOfFixedSubTimeSteps;
}

void simAnimation::advanceSingleFrame()
{
    ++mCurrentFrame;
    update(mCurrentFrame);
}

frame simAnimation::currentFrame() const
{
    return mCurrentFrame;
}

void simAnimation::setCurrentFrame(const frame& frame)
{
    mCurrentFrame = frame;
}

double simAnimation::currentTimeInSeconds() const
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