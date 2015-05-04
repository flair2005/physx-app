#pragma once

#include "DefHeaders.h"

class Timer {
private:
	static double countsPerSecond;
	static __int64 CounterStart;
	static __int64 frameTimeOld;
public:
	static void startTimer();
	static double getTime();
	static double getFrameTime();
};