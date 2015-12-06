﻿#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

#include <windows.h> // подключение библиотеки с функциями API

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int IntRound( double _value );

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

#endif //_SUPPORT_HPP_