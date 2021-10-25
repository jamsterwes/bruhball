#pragma once
#include "../Common.h"
#include "../Engine.h"

class VibrationScript : public IScript
{
public:
	virtual void Initialize(Engine* engine);
	virtual void Update(Engine* engine, const FrameState& state);
	virtual void Destroy(Engine* engine);
private:
	XINPUT_VIBRATION _vibrationState;
};