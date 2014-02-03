//***************************************************************************************
// Modified version of GameTimer.h by Frank Luna (C) 2011.
//***************************************************************************************

#pragma  once

#include "Precision.h"

namespace engiX
{
    class Timer
    {
    public:
        Timer();

        real TotalTime() const;  // in seconds
        real DeltaTime() const; // in seconds

        void Reset(); // Call before message loop.
        void Start(); // Call when unpaused.
        void Stop();  // Call when paused.
        void Tick();  // Call every frame.

    private:
        real mSecondsPerCount;
        real mDeltaTime;

        __int64 mBaseTime;
        __int64 mPausedTime;
        __int64 mStopTime;
        __int64 mPrevTime;
        __int64 mCurrTime;

        bool mStopped;
    };
}