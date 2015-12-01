#include "support.hpp"

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "lab3: input";
const SIZE wndDefaultSize{ 600, 400 };

// �������� ��������� 
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd )
{
	MSG msg;

	// ����������� ������ ���� 
	MyRegisterClass( hInstance );

	// �������� ���� ����������
	if ( !InitInstance( hInstance, nShowCmd ) )
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
	wcex.hIcon = LoadIcon( NULL, IDI_INFORMATION );		// ����������� ������
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );    // ����������� �������
	wcex.hbrBackground = GetSysColorBrush( COLOR_WINDOW );   // ��������� ����
	wcex.lpszMenuName = NULL;		// ����������� ����
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

	const RECT screenField{
		0, 0,
		GetSystemMetrics( SM_CXSCREEN ),
		GetSystemMetrics( SM_CYSCREEN )
	};

	const POINT wndPos = GetCenternedPosition( wndDefaultSize, screenField );

	hWnd = CreateWindow( szWindowClass, // ��� ������ ����
		szTitle,   // ��� ����������
		WS_MINIMIZEBOX | WS_TILED | WS_SIZEBOX | WS_SYSMENU, // ����� ����
		wndPos.x,	// ��������� �� �
		wndPos.y, 	// ��������� �� Y
		wndDefaultSize.cx,    // ������ �� �
		wndDefaultSize.cy,    // ������ �� Y
		NULL,	// ��������� ������������� ����
		NULL,       // ��������� ���� ����
		hInstance,  // ��������� ����������
		NULL );     // ��������� ��������.

	if ( !hWnd ) // ���� ���� �� ���������, ������� ���������� FALSE
		return FALSE;

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

	static POINT printOutCoordinates = { 0, 0 };

	switch ( message )
	{
	// ��������� �������� ��� �������� ����
	case WM_CREATE: 
		break;

	// ������������ ����
	case WM_PAINT:
		// ������ ����������� �����
		hdc = BeginPaint( hWnd, &ps );

		char buff[ 60 ];

		// ����� ����������
		TextOut( hdc, printOutCoordinates.x, printOutCoordinates.y, buff, wsprintf( buff, "Current coordinates: %dx%d", printOutCoordinates.x, printOutCoordinates.y ) );

		// ��������� ����������� �����
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY: // ���������� ������
		PostQuitMessage( 0 );
		break;

	// �� ������� ����� �� ������ ���� ��������� ����� ����������
	case WM_LBUTTONUP: case WM_MBUTTONUP: case WM_RBUTTONUP:
		printOutCoordinates.x = LOWORD( lParam );
		printOutCoordinates.y = HIWORD( lParam );
		InvalidateRect( hWnd, NULL, TRUE );
		break;

	default:
		// ��������� ���������, ������� �� ���������� �������������
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}