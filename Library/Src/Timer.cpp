#include "Timer.hpp"

#include "Helpers.hpp"

#include <exception>

Timer::Timer()
{
    BOOL qpfOk = ::QueryPerformanceFrequency(&mFrequency);
    ThrowIfFalse(qpfOk, QueryPerformanceFrequency);

    BOOL qpcOk = ::QueryPerformanceCounter(&mStartTime);
    ThrowIfFalse(qpcOk, QueryPerformanceCounter);

    mCurrentTime = mStartTime;
    mLastTime = mStartTime;
}

TimerTime Timer::GetTime()
{
    ::QueryPerformanceCounter(&mCurrentTime);

    TimerTime time = {
        double(mCurrentTime.QuadPart - mLastTime.QuadPart) / mFrequency.QuadPart, // elapsed time
        double(mCurrentTime.QuadPart - mStartTime.QuadPart) / mFrequency.QuadPart // total time
    };

    mLastTime = mCurrentTime;

    return time;
}
