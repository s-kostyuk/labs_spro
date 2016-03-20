#include "painter.hpp"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {
	return TRUE;
}

L7_2_DLL void paint_star( HWND hWnd, RECT *r ) {

	HDC hdc = GetDC( hWnd );

	const short N_OF_POINTS = 5;

	POINT pt[ N_OF_POINTS ] = { 600, 50, 640, 200, 490, 100, 650, 100, 500, 200 };

	Polygon( hdc, pt, N_OF_POINTS );

	ReleaseDC( hWnd, hdc );
}