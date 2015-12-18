#ifndef _MAIN_HPP_
#define _MAIN_HPP_

/*****************************************************************************/

#include <windows.h> // подключение библиотеки с функциями API

#include "btn_controller.hpp"
#include "figures.hpp"

/*****************************************************************************/

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

/*****************************************************************************/

#endif // _MAIN_HPP_