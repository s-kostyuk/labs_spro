#include "painter.h"
#include "poly_lib.h"

/**********************************************************************************/

void Init5PointStar( POINT * _p, const RECT * _pRect );
void PolyToStarHardcored( POINT * _pStar, const POINT * _pPoly );

/**********************************************************************************/

#define N_OF_POINTS 5

/**********************************************************************************/

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {
	return TRUE;
}

/**********************************************************************************/

L7_2_DLL void Paint5PointStar( HDC _hdc, const RECT * _pRect ) {

	POINT points[ N_OF_POINTS ];

	Init5PointStar( points, _pRect );

	Polygon( _hdc, points, N_OF_POINTS );

}

/**********************************************************************************/

L7_2_DLL void PaintPentagon( HDC _hdc, const RECT * _pRect ) {

	POINT points[ N_OF_POINTS ];

	InitRegularPoligon( points, N_OF_POINTS, _pRect );

	Polygon( _hdc, points, N_OF_POINTS );

}

/**********************************************************************************/

L7_2_DLL void PaintTestStar( HWND hWnd ) {

	HDC hdc = GetDC( hWnd );

	POINT points[ N_OF_POINTS ] = { 600, 50, 640, 200, 490, 100, 650, 100, 500, 200 };

	Polygon( hdc, points, N_OF_POINTS );

	ReleaseDC( hWnd, hdc );

}

/**********************************************************************************/

void Init5PointStar( POINT * _p, const RECT * _pRect ) {

	POINT pentagonPoints[ N_OF_POINTS ];

	InitRegularPoligon( pentagonPoints, N_OF_POINTS, _pRect );

	PolyToStarHardcored( _p, pentagonPoints );

}

/**********************************************************************************/

void PolyToStarHardcored( POINT * _pStar, const POINT * _pPoly ) {
	_pStar[ 0 ] = _pPoly[ 0 ];
	_pStar[ 1 ] = _pPoly[ 3 ];
	_pStar[ 2 ] = _pPoly[ 1 ];
	_pStar[ 3 ] = _pPoly[ 4 ];
	_pStar[ 4 ] = _pPoly[ 2 ];
}
