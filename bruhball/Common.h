#pragma once


/****  HEADER FILES  ****/


// Disable lean and mean
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

// Windows Header Files:
#include <Windows.h>
#include <windowsx.h>

// Direct2D Header Files:
#include <d2d1_3helper.h>
#include <d2d1svg.h>

// DXGI Header Files:
#include <dxgi1_6.h>

// D2D Text Rendering Header Files:
#include <dwrite_3.h>

// Windows Imaging Header Files:
#include <wincodecsdk.h>

// XInput Header Files:
#include <Xinput.h>

// C/C++ Header Files:
#include <cstdlib>
#include <cwchar>
#include <cmath>
#include <chrono>
#include <concepts>
#include <list>


/****  MACROS  ****/

#include "Common/Macros.h"


/****  LIBRARY FILES  ****/


#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "xinput.lib")