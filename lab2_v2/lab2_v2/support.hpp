#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

#include <windows.h> // подключение библиотеки с функциями API

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int IntRound( double _value );

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

RECT GetOuterEllipse( const RECT & _innerDim, const short _nOfPolygonPoints );

void DrawRegularConvexPoligon( HDC & _hdc, const short _nOfPoints, const RECT & _ellipse );

void InitRegularPoligon( POINT * _pPoints, const short _nOfPoints, const RECT & _ellipse );

#endif //_SUPPORT_HPP_