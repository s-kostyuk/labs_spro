#include "support.hpp"

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "lab2: metrics";
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

void PrintMetrics( HWND & _hWnd, HDC & _hdc, const TEXTMETRIC & _tm ) {
	// ������� ���� ��� ���������
	RECT clientRect;
	GetClientRect( _hWnd, &clientRect );

	// ��������� �����
	char buff[ 60 ];

	// ������ ����� ������������ ���� ����� (���������� ��������)
	const short padding = IntRound( _tm.tmHeight * 1.5 );

	// ������������ ����������, ��������� ������� - �������� ������ ������
	long currYPos = _tm.tmHeight >> 1;

	// ����� ������� ������
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Icon size: %dx%dpx.", GetSystemMetrics( SM_CXICON ), GetSystemMetrics( SM_CYICON )));
	
	// ����� ���������� �...
	currYPos += padding;
	// ...������ �������� �����
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Window border width: %dpx, height: %dpx.", GetSystemMetrics( SM_CXBORDER ), GetSystemMetrics( SM_CYBORDER )));
	
	// ������� �� ������ ���� ���������� �������
	currYPos = clientRect.bottom - 2 * padding;

	// ����� ���������� ��������� ��������
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Resolution of the main monitor: %dx%dpx.", GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN )));
	
	// ����� ���������� �...
	currYPos += padding;
	// ...������ ������� ������ �������
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Average char width: %d, line spacing: %d.", _tm.tmAveCharWidth, _tm.tmExternalLeading ) );

	/* � ����� ��������� tm:
	 * tmExternalLeading - ����������� ����������� ��������, ������������� ������������� ������;
	 * tmInternalLeading - ������ ����������� ��������� ����,
	 * tmHeight - ������ ���� � ������ ����������� ���������.
	 */
}

void DrawRegularConvexPoligon( HDC & _hdc, const short _nOfPoints, const RECT & _ellipse ) {
	POINT * pPoints = new POINT[ _nOfPoints ];

	InitRegularPoligon( pPoints, _nOfPoints, _ellipse );

	Polygon( _hdc, pPoints, _nOfPoints );

	delete[] pPoints;
}

void PrintPolygons( HWND & _hWnd, HDC & _hdc ) {
	RECT clientRect, windowRect;

	// ������� ���� ��� ���������
	GetClientRect( _hWnd, &clientRect );

	// ������� ����
	GetWindowRect( _hWnd, &windowRect );

	// ���������� ���������� �����
	HBRUSH hRedBrush, hGreenBrush, hOldBrush;

	// �������� ������
	hRedBrush   = CreateSolidBrush( RGB( 255, 0, 0 ) );
	hGreenBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );

	// ����� ����� � ��������
	hOldBrush = (HBRUSH)SelectObject( _hdc, hRedBrush ); 

	// ��������� ���������� �������
	SIZE ellipseSize {
		IntRound( ( windowRect.right - windowRect.left ) * 250.0 / wndDefaultSize.cx ),
		IntRound( ( windowRect.bottom - windowRect.top ) * 100.0 / wndDefaultSize.cy ),
	};

	// ������� ���������� �������
	POINT ellipsePos = GetCenternedPosition( ellipseSize, clientRect );

	// ���������� ���������� ������ ������� ��������������
	//left top right bottom
	RECT epsDimensions {
		ellipsePos.x,
		ellipsePos.y,
		ellipsePos.x + ellipseSize.cx,
		ellipsePos.y + ellipseSize.cy
	};

	// ������ ������������
	const short nOfPolygonPoints = 5;

	// ��������� ������� ���������� �������
	RECT outerEllipse = GetOuterEllipse( epsDimensions, nOfPolygonPoints );

	// ������ ������� (������������, ��. ���. 159)
	DrawRegularConvexPoligon( _hdc, nOfPolygonPoints, outerEllipse );

	// ������ �����
	SelectObject( _hdc, hGreenBrush );

	// ������ ������
	Ellipse( _hdc, epsDimensions.left, epsDimensions.top, epsDimensions.right, epsDimensions.bottom );

	// �������� ������ �����
	SelectObject( _hdc, hOldBrush );

	// ������� ��������� �����
	DeleteObject( hRedBrush );
	DeleteObject( hGreenBrush );
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  ������� ���������. ��������� � ������������ ��� ���������, ���������� � ����������
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	static TEXTMETRIC tm;

	switch ( message )
	{
	// ��������� �������� ��� �������� ����
	case WM_CREATE: 
		hdc = BeginPaint( hWnd, &ps );
		GetTextMetrics( hdc, &tm );
		EndPaint( hWnd, &ps );
		break;

	// ������������ ����
	case WM_PAINT:
		// ������ ����������� �����
		hdc = BeginPaint( hWnd, &ps );

		// ����� ����������
		PrintMetrics( hWnd, hdc, tm );
		PrintPolygons( hWnd, hdc );

		// ��������� ����������� �����
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY: // ���������� ������
		PostQuitMessage( 0 );
		break;

	default:
		// ��������� ���������, ������� �� ���������� �������������
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}