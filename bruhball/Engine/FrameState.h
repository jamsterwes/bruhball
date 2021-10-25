#pragma once
#include "../Common.h"

struct FrameState
{
	// == Storing input
	XINPUT_STATE** ControllerStates = nullptr;

	// == Keeping time
	double Time = 0.f;       // time in seconds since start of application
	double DeltaTime = 0.f;  // time in seconds since last frame
};