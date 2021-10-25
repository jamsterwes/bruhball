#pragma once
#include "Common.h"

// Engine includes
#include "Engine/FrameState.h"
#include "Engine/RenderState.h"
#include "Engine/IScript.h"

class Engine
{
public:
	Engine(HINSTANCE hinst);
	~Engine();

	void OnPreUpdate();
	bool OnRender();
	void OnResize(UINT width, UINT height);

	void Run();

	template<class T> requires std::derived_from<T, IScript> && std::default_initializable<T>
	void AddScript()
	{
		T* script = new T();
		script->Initialize(this);
		_scripts.push_back(script);
	}

private:
	void XInput_SetupControllers();
	bool Win32_CreateApplication();
	bool D2D_CreateDeviceIndependentResources();
	bool D2D_CreateDeviceDependentResources();

	void D2D_DiscardResources();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// Win32 Handles
	HWND _hwnd;
	HINSTANCE _hinst;

	// Direct2D Handles
	ID2D1Factory* _d2dFactory;
	ID2D1HwndRenderTarget* _renderTarget;

	// State keeping
	RenderState _stateRender;
	FrameState _stateFrame;
	std::chrono::high_resolution_clock::time_point _startTime;

	// Scripts
	std::list<IScript*> _scripts;
};