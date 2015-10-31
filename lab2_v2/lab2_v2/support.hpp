#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

#include <windows.h> // подключение библиотеки с функциями API

enum EllipseType {
	INNER, OUTER
};

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int IntRound( double _value );

POINT && GetCenternedPosition( const SIZE & _object, const RECT & _field );

void PrintRegularConvexPoligon( HDC & _hdc, const short _nOfPoints, const RECT & _ellipse, EllipseType _ellipseType = OUTER );

void InitRegularPoligon( POINT * _pPoints, const short _nOfPoints, const RECT & _ellipse, EllipseType _ellipseType = OUTER );

#endif //_SUPPORT_HPP_