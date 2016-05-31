#include <windows.h>
HINSTANCE hInst;
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "Прозрачные окна";
HWND hWnd;
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	MSG msg;

	MyRegisterClass( hInstance );

	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}

	while ( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}

ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush( RGB( 255, 255, 255 ) );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassEx( &wcex );
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{

	hInst = hInstance;
	hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInst, NULL );

	if ( !hWnd )
	{
		return FALSE;
	}
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
	return TRUE;
}

HWND selected_hwnd, prev_hwnd;
HHOOK hk;
LRESULT CALLBACK MouseProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	MSLLHOOKSTRUCT* pm = (MSLLHOOKSTRUCT*)lParam;

	selected_hwnd = WindowFromPoint( pm->pt );
	selected_hwnd = GetAncestor( selected_hwnd, GA_ROOT );

	SetWindowLong( selected_hwnd, GWL_EXSTYLE, GetWindowLong( hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	SetLayeredWindowAttributes( selected_hwnd, 0, 185, LWA_ALPHA );
	if ( selected_hwnd != prev_hwnd )
	{
		SetLayeredWindowAttributes( prev_hwnd, 0, 255, LWA_ALPHA );
	}
	prev_hwnd = selected_hwnd;

	return CallNextHookEx( hk, nCode, wParam, lParam );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush;
	switch ( message )
	{

	case WM_CREATE:
		hk = SetWindowsHookEx( WH_MOUSE_LL, MouseProc, hInst, 0 );//создание хука
		break;

	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );

		EndPaint( hWnd, &ps );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}
