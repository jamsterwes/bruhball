#include "VibrationScript.h"

void VibrationScript::Initialize(Engine* engine)
{
	ZeroMemory(&_vibrationState, sizeof(XINPUT_VIBRATION));
}

void VibrationScript::Update(Engine* engine, const FrameState& state)
{
	_vibrationState.wRightMotorSpeed = GetControllerButton(state, 0, XINPUT_GAMEPAD_A) ? 32000 : 0;
	XInputSetState(0, &_vibrationState);
}

void VibrationScript::Destroy(Engine* engine)
{

}