#include <windows.h>	// ����������� ���������� � ��������� API

#include "resource.h"	// ��������� ����

#include "../painter/painter.h" // header-���� ����������

#include <assert.h> 
#include <windowsx.h>
#include <tchar.h>

/**********************************************************************************/

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
PTCHAR szWindowClass = _T("Kostyuk");
PTCHAR szTitle = _T("lab7_v2: Star painter");

/**********************************************************************************/

// ��������������� �������� �������
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

/**********************************************************************************/

// �������� ��������� 
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PTCHAR    lpCmdLine,
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

/**********************************************************************************/

//  ����������� ������ ���� 
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

/**********************************************************************************/

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

/**********************************************************************************/

//  ������� ���������. ��������� � ������������ ��� ���������, ���������� � ����������
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	static RECT currObjectDim; // ������� �������� �������

	// ��� ��������� �� �������, ������� ����� ����������� ��������� ������� �������� RECT � ��������� HDC
	typedef void ( * PainterFunc )( HDC, const RECT * );

	// "����������" - ��������� �� ������� ������� ��������� 
	static PainterFunc fPainter = PaintPentagon;

	// ��������� ���������
	switch ( message )
	{
	case WM_CREATE: // ��������� �������� ��� �������� ����
		break;

	case WM_LBUTTONDOWN: // ������� �� ����� ������ ����
		// ��������� ����������
		// ����� ������� ���� ��������� �������������. �� ��������� �������, ��� ������ ����� ����� ������� ������
		currObjectDim.left = currObjectDim.right = GET_X_LPARAM( lParam );
		currObjectDim.top = currObjectDim.bottom = GET_Y_LPARAM( lParam );

		// ����������� ����������� ����
		SetCapture( hWnd );

		break;

	case WM_MOUSEMOVE: // ������������ ������� ����
		// ���� �� ����������� ���� � ������� ����� �������
		if ( wParam & MK_LBUTTON ) {
			// ���������� ����� ���������� ������� �������� ����
			currObjectDim.right  = GET_X_LPARAM( lParam );
			currObjectDim.bottom = GET_Y_LPARAM( lParam );

			// �������������� ���������� ����
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;

	case WM_LBUTTONUP: // "�������" ����� ������ ����
		// ����������� ����
		ReleaseCapture();
		break;

	case WM_PAINT:  // ������������ ����
		hdc = BeginPaint( hWnd, &ps );	// ������ ����������� �����
		GetClientRect( hWnd, &rt ); // ������� ���� ��� ���������

		assert( fPainter != NULL ); // �������������: ��������� �� ������� fPainter �� ������ ���� NULL

		fPainter( hdc, &currObjectDim ); // ����� ������� ������� ���������

		EndPaint( hWnd, &ps );	// ��������� ����������� �����
		break;

	case WM_DESTROY: // ���������� ������
		PostQuitMessage( 0 );
		break;

	case WM_COMMAND: // ��������� ��������� �� ����
		switch ( LOWORD( wParam ) ) {
		case ID_CHANGE_STAR:
			fPainter = Paint5PointStar; // ������� ��������� - ������� ��������� ������
			break;

		case ID_CHANGE_POLYGON:
			fPainter = PaintPentagon; // ������� ��������� - ������� ��������� �������������
			break;

		case ID_PAINT_STAR:
			PaintTestStar( hWnd ); // ������ �������� ������ �� ����� ������
			break;
		}
		break;

	default:
		// ��������� ���������, ������� �� ���������� �������������
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}