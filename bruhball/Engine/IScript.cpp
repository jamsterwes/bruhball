#include "../Common.h"
#include "IScript.h"
#include "FrameState.h"

bool IScript::GetControllerButton(const FrameState& state, int user, short button)
{
	if (state.ControllerStates[user] == nullptr) return false;
	return state.ControllerStates[user]->Gamepad.wButtons & button;
}