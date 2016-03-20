#pragma once

#ifndef L7_2_DLL

#define L7_2_DLL __declspec(dllexport)

#endif // L7_2_DLL

/**********************************************************************************/

#include <windows.h>

/**********************************************************************************/

L7_2_DLL void PaintTestStar  ( HWND hWnd );

L7_2_DLL void Paint5PointStar( HDC  _hdc, const RECT * _pRect );

L7_2_DLL void PaintPentagon  ( HDC  _hdc, const RECT * _pRect );
