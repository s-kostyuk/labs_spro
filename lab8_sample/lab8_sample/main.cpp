#include <windows.h>         // ����������� ���������� � ��������� API
#include <time.h>

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "������ ���������";

int arr1[ 3 ][ 3 ] = { { 10, 12, 33 },{ 24, 52, 46 },{ 17, 28, 95 } };
int arr2[ 3 ][ 3 ] = { { 3, 2, 1 },{ 6, 5, 4 },{ 9, 8, 7 } };
int arr3[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr4[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr5[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr6[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr7[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };

void do_stuff() {
	srand( time( NULL ) );
	int x = rand() % 2;

	__asm {
		jmp global_start

		global_start :
			mov ecx, 9; comment
		
		r1 :
			mov eax, arr1[ ecx * 4 - 4 ]; ��������� ������� ������� arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; ��������� ������� ������� arr2
			add eax, ebx; �������� ��������� ������� 1 � 2
			mov arr3[ ecx * 4 - 4 ], eax; ����������� ���������� � ������ ������
			loop r1
			
			; --------------------------------------
			
			mov ecx, 9; comment
		r2 :
			mov eax, arr1[ ecx * 4 - 4 ]; ��������� ������� ������� arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; ��������� ������� ������� arr2
			sub eax, ebx; ������� ��������� ������� 1 � 2
			mov arr4[ ecx * 4 - 4 ], eax; ����������� ���������� � ��������� ������
			loop r2
			
			; -------------------------------------- 
			
			mov ecx, 9; comment
		r3 :
			mov eax, arr1[ ecx * 4 - 4 ]; ��������� ������� ������� arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; ��������� ������� ������� arr2
			mul ebx; ��������� ebx � aex
			mov arr5[ ecx * 4 - 4 ], eax; ����������� ���������� � ����� ������
			loop r3

			; --------------------------------------

			mov ecx, 9; comment
		r4 :
			mov eax, arr1[ ecx * 4 - 4 ]; ��������� ������� ������� arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; ��������� ������� ������� arr2
			div bl;
			mov arr6[ ecx * 4 - 4 ], al; ����������� ���������� � ������ ������
			mov arr7[ ecx * 4 - 4 ], ah; ����������� ���������� � ������� ������
			loop r4
	}
}

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

void PrintArray( HDC _hdc, const RECT & _client, int _array[3][3], int _padding_x, int _padding_y, char * _buf ) {

	for ( int row = 0; row < 3; ++row ) {
		TextOut( _hdc, _padding_x, 20 * row + _padding_y + 20, _buf, wsprintf( _buf, "%d %d %d", _array[ row ][0], _array[ row ][ 1 ], _array[ row ][ 2 ] ) );
	}

}

void PrintArrays( HDC _hdc, const RECT & _client ) {

	// ��������� �����
	char buff[ 60 ];

	int padding_y = 10;
	int padding_x = 10;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 1:") );
	PrintArray( _hdc, _client, arr1, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 2:") );
	PrintArray( _hdc, _client, arr2, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 3:") );
	PrintArray( _hdc, _client, arr3, padding_x, padding_y, buff );

	padding_x = 10;
	padding_y += 110;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 4:") );
	PrintArray( _hdc, _client, arr4, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 5:") );
	PrintArray( _hdc, _client, arr5, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 6:") );
	PrintArray( _hdc, _client, arr6, padding_x, padding_y, buff );

	padding_x = 10;
	padding_y += 110;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Array 7:") );
	PrintArray( _hdc, _client, arr7, padding_x, padding_y, buff );

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
		do_stuff();
		break;

	case WM_PAINT:  // ������������ ����
		hdc = BeginPaint( hWnd, &ps );	// ������ ����������� �����
		GetClientRect( hWnd, &rt ); // ������� ���� ��� ���������

		PrintArrays( hdc, rt );

		EndPaint( hWnd, &ps );	// ��������� ����������� �����
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