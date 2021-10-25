#include "../Engine.h"

Engine::Engine(HINSTANCE hinst) : 
	_hinst(hinst),
    _hwnd(nullptr),
	_d2dFactory(nullptr),
	_renderTarget(nullptr),
	_stateRender(),
	_stateFrame(),
	_scripts()
{
	// Get start time
	_startTime = std::chrono::high_resolution_clock::now();

	XInput_SetupControllers();
	D2D_CreateDeviceIndependentResources();
	Win32_CreateApplication();
}

Engine::~Engine()
{
	// Destroy scripts
	for (IScript* script : _scripts)
	{
		script->Destroy(this); 
		delete script;
	}

	SafeRelease(_d2dFactory);
	D2D_DiscardResources();
}

void Engine::XInput_SetupControllers()
{
	// Poll/allocate for controllers
	_stateFrame.ControllerStates = new XINPUT_STATE * [4];
	XINPUT_STATE state;
	DWORD dwResult;
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		dwResult = XInputGetState(i, &state);
		_stateFrame.ControllerStates[i] = (dwResult == ERROR_SUCCESS) ? new XINPUT_STATE : nullptr;
	}
}

bool Engine::Win32_CreateApplication()
{
	// Initialize Window
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Engine::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = _hinst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"BBWindow";

	RegisterClassEx(&wcex);

	_hwnd = CreateWindow(
		wcex.lpszClassName,
		L"bruhball",  // TODO: move to a settings object
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,  // TODO: move to a settings object
		480,  // TODO: move to a settings object
		NULL,
		NULL,
		wcex.hInstance,
		this
	);

	if (!_hwnd) return false;

	ShowWindow(_hwnd, SW_SHOWNORMAL);
	UpdateWindow(_hwnd);

	return true;
}

void GetControllerStates(FrameState& fstate)
{
	// Copy states
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XInputGetState(i, fstate.ControllerStates[i]);
	}
}

void Engine::OnPreUpdate()
{
	// Set times
	std::chrono::duration<double, std::milli> newDuration = std::chrono::high_resolution_clock::now() - _startTime;
	_stateFrame.DeltaTime = newDuration.count() - _stateFrame.Time;
	_stateFrame.Time = newDuration.count();

	// Set controller states
	GetControllerStates(_stateFrame);
}

bool Engine::OnRender()
{
	if (!D2D_CreateDeviceDependentResources()) return false;

	HRESULT hr = S_OK;


	_renderTarget->BeginDraw();

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->Clear(_stateRender.ClearColor);

	hr = _renderTarget->EndDraw();


	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		D2D_DiscardResources();
	}

	return true;
}

void Engine::OnResize(UINT width, UINT height)
{
	if (_renderTarget)
	{
		_renderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void Engine::Run()
{
	MSG msg;
	msg.message = WM_NULL;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			// Update

			OnPreUpdate();
			for (IScript* script : _scripts) script->Update(this, _stateFrame);

			// Render

			OnRender();
		}
	}
}

bool Engine::D2D_CreateDeviceIndependentResources()
{
	// Initialize COM
	if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) return false;

	// Initialize D2D Factory
	if (!SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory))) return false;

	return true;
}

bool Engine::D2D_CreateDeviceDependentResources()
{
	if (_renderTarget) return true;

	RECT rc;
	GetClientRect(_hwnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	
	HRESULT hr = _d2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_hwnd, size),
		&_renderTarget);

	if (!SUCCEEDED(hr)) return false;

	return true;
}

void Engine::D2D_DiscardResources()
{
	SafeRelease(_renderTarget);
}

LRESULT CALLBACK Engine::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Take pointer to Engine from CreateWindow,
	// and move it into the WindowLongPtr
	// (maintain association between HWND and Engine instance)
	if (msg == WM_CREATE)
	{
		LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		Engine* engine = reinterpret_cast<Engine*>(pcs->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(engine));

		return 1;
	}

	// Otherwise, retrieve Engine instance from the WindowLongPtr
	Engine* engine = reinterpret_cast<Engine*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (engine)
	{
		// WRITE MESSAGE HANDLERS HERE
		switch (msg)
		{
		case WM_SIZE:
			engine->OnResize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_DISPLAYCHANGE:
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		case WM_PAINT:
			if (!engine->OnRender())
			{
				PostQuitMessage(0);
				return 1;
			}
			ValidateRect(hwnd, NULL);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 1;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}