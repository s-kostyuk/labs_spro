#pragma once

#ifdef L7_2_DLL

#else

#define L7_2_DLL __declspec(dllexport)

#endif // L7_2_DLL

#include <windows.h>

L7_2_DLL void paint_star( HWND hWnd, RECT *r );

