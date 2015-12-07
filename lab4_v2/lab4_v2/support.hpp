#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

#include <windows.h> // подключение библиотеки с функциями API

#include <vector>

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int IntRound( double _value );
POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

enum class ObjectType {
	LINE,
	ELLIPSE,
	RECTANGLE
};

void DrawSavedObjects( HDC _hdc, const std::vector< std::pair < ObjectType, RECT > > & _saved );

void DrawObject( HDC _hdc, const RECT & _objectCoordinates, const ObjectType _type );

#endif //_SUPPORT_HPP_