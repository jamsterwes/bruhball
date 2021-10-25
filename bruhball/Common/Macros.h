#pragma once

#ifndef SafeRelease
#define SafeRelease(v) if(v) { v->Release(); v = nullptr; }
#endif