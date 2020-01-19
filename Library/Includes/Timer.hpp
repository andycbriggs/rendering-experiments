#pragma once

#include <Windows.h>

struct TimerTime
{
	const double intervalTime;
	const double totalTime;
};

class Timer
{
public:
    Timer();

    TimerTime GetTime();

private:
    LARGE_INTEGER mFrequency;

    LARGE_INTEGER mStartTime;
    LARGE_INTEGER mCurrentTime;
    LARGE_INTEGER mLastTime;
};