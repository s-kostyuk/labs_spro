#include "painter.h"
#include "poly_lib.h"

/**********************************************************************************/

void Init5PointStar( POINT * _p, const RECT * _pRect );
void PolyToStarHardcored( POINT * _pStar, const POINT * _pPoly );

/**********************************************************************************/

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {
	return TRUE;
}

/**********************************************************************************/

L7_2_DLL void Paint5PointStar( HDC _hdc, const RECT * _pRect ) {

	const short nOfPoints = 5;

	POINT points[ nOfPoints ];

	Init5PointStar( points, _pRect );

	Polygon( _hdc, points, nOfPoints );

}

/**********************************************************************************/

L7_2_DLL void PaintPentagon( HDC _hdc, const RECT * _pRect ) {

	const short nOfPoints = 5;

	POINT points[ nOfPoints ];

	InitRegularPoligon( points, nOfPoints, _pRect );

	Polygon( _hdc, points, nOfPoints );

}

/**********************************************************************************/

L7_2_DLL void PaintTestStar( HWND hWnd ) {

	const short nOfPoints = 5;

	HDC hdc = GetDC( hWnd );

	POINT points[ nOfPoints ] = { 600, 50, 640, 200, 490, 100, 650, 100, 500, 200 };

	Polygon( hdc, points, nOfPoints );

	ReleaseDC( hWnd, hdc );

}

/**********************************************************************************/

void Init5PointStar( POINT * _p, const RECT * _pRect ) {

	const short nOfPoints = 5;

	POINT pentagonPoints[ nOfPoints ];

	InitRegularPoligon( pentagonPoints, nOfPoints, _pRect );

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
