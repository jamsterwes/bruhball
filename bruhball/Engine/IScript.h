#pragma once

class IScript
{
public:
	virtual void Initialize(class Engine* engine) = 0;
	virtual void Update(class Engine* engine, const struct FrameState& state) = 0;
	virtual void Destroy(class Engine* engine) = 0;
protected:
	bool GetControllerButton(const struct FrameState& state, int user, short button);
};