#ifndef _MAIN_HPP_
#define _MAIN_HPP_

/*****************************************************************************/

#include <windows.h> // ����������� ���������� � ��������� API

#include "btn_controller.hpp"
#include "figures.hpp"

/*****************************************************************************/

// ��������������� �������� �������
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

/*****************************************************************************/

#endif // _MAIN_HPP_