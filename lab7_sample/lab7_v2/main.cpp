#include <windows.h>         // ����������� ���������� � ��������� API
#include <tchar.h>

#include "resource.h"

#include "../painter/painter.hpp"

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = _T("Kostyuk");
LPCTSTR szTitle = _T("lab7_v2: Star painter");

// ��������������� �������� �������

ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

// �������� ��������� 
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	MSG msg;

	// ����������� ������ ���� 
	MyRegisterClass( hInstance );

	// �������� ���� ����������
	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}
	// ���� ��������� ���������
	while ( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  ������������ ����� ���� 

ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// ����� ����
	wcex.lpfnWndProc = (WNDPROC)WndProc; // ������� ���������
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;		// ��������� ����������
	wcex.hIcon = LoadIcon( NULL, IDI_HAND );		// ����������� ������
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );    // ����������� �������
	wcex.hbrBackground = GetSysColorBrush( COLOR_WINDOW );   // ��������� ����
	wcex.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );		// ����������� ����
	wcex.lpszClassName = szWindowClass;	// ��� ������
	wcex.hIconSm = NULL;

	return RegisterClassEx( &wcex ); // ����������� ������ ����
}

// FUNCTION: InitInstance(HANDLE, int)
// ������� ���� ���������� � ��������� ��������� ���������� � ���������� hInst

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWnd;

	hInst = hInstance; // ��������� ��������� ���������� � ���������� hInst

	hWnd = CreateWindow( szWindowClass, // ��� ������ ����
		szTitle,   // ��� ����������
		WS_OVERLAPPEDWINDOW, // ����� ����
		CW_USEDEFAULT,	// ��������� �� �
		CW_USEDEFAULT, 	// ��������� �� Y
		CW_USEDEFAULT,    // ������ �� �
		CW_USEDEFAULT,    // ������ �� Y
		NULL,	// ��������� ������������� ����
		NULL,       // ��������� ���� ����
		hInstance,  // ��������� ����������
		NULL );     // ��������� ��������.

	if ( !hWnd ) // ���� ���� �� ���������, ������� ���������� FALSE
	{
		return FALSE;
	}
	ShowWindow( hWnd, nCmdShow );		// �������� ����
	UpdateWindow( hWnd );			// �������� ����
	return TRUE;				//�������� ���������� �������
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  ������� ���������. ��������� � ������������ ��� ���������, ���������� � ����������


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch ( message )
	{
	case WM_CREATE: // ��������� �������� ��� �������� ����
		break;

	case WM_PAINT:  // ������������ ����
		hdc = BeginPaint( hWnd, &ps );	// ������ ����������� �����
		GetClientRect( hWnd, &rt ); // ������� ���� ��� ���������
		DrawText( hdc, _T("������, ���!"), -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
		EndPaint( hWnd, &ps );	// ��������� ����������� �����
		break;

	case WM_DESTROY: // ���������� ������
		PostQuitMessage( 0 );
		break;

	case WM_COMMAND: // ��������� ����
		switch ( LOWORD( wParam ) ) {
		case ID_CHANGE_STAR:
			paint_star( hWnd, nullptr );
			break;

		}
		break;

	default:
		// ��������� ���������, ������� �� ���������� �������������
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}