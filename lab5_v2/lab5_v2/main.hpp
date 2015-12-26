#ifndef _MAIN_HPP_
#define _MAIN_HPP_

/*****************************************************************************/

#include "support.hpp"
#include "threads.hpp"

#include <windows.h> // подключение библиотеки с функциями API
#include <Tchar.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>

/*****************************************************************************/

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

/*****************************************************************************/

#endif // _MAIN_HPP_
