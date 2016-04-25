#include <windows.h>         // ����������� ���������� � ��������� API

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "Intro to assembler";

const int N_OF_BITS = 32; // ������ ����� �� ������� ������
CHAR char_buf[ N_OF_BITS + 1 ]; // ������-����� ��� �������� ���������� ������������� �����
UINT32 reg_copy_before[ 4 ];
UINT32 reg_copy_after[ 4 ];

void int_to_cstring(int _num) {

	__asm {
		jmp global_start

		append_zero :
			mov char_buf[ ecx - 1 ], '0'; ���������� ��������
			jmp after_append

		append_one :
			mov	char_buf[ ecx - 1 ], '1'; ���������� ��������
			jmp after_append

		global_start :
			mov ecx, N_OF_BITS; ������� � ������� ���������� ����� ��� ���������
			mov	char_buf[ ecx ], 0; ��������� ������������ ���� � ����� �������
			mov eax, _num; ������� �������� ����� � �����������

		convert_it:
			mov edx, eax; �������� ����� � ����������� edx
			and edx, 1; ��������� �������� ���, ��������� ���������� � ���������� � edx
			cmp edx, 0; ���������� ����� � �����
			je append_zero; ���� ��� ���� � ������� ������� - ���������� ����
			jne append_one; ���� ���� ������� - ���������� �������

		after_append : ; ����� ������ �����...
			sar eax, 1; �������� �����
			loop convert_it; ������������ � ������ �����
	}
}

void subs() {

	__asm {
		mov eax, 1024
		mov edx, 2015

		mov reg_copy_before[ 0 ], eax
		mov reg_copy_before[ 4 ], ebx
		mov reg_copy_before[ 8 ], ecx
		mov reg_copy_before[ 12 ], edx

		sub eax, edx

		mov reg_copy_after[ 0 ], eax
		mov reg_copy_after[ 4 ], ebx
		mov reg_copy_after[ 8 ], ecx
		mov reg_copy_after[ 12 ], edx
	}

}

void divs() {

	__asm {
		mov eax, 1024
		mov edx, 2015
		mov ebx, 2

		mov reg_copy_before[ 0 ], eax
		mov reg_copy_before[ 4 ], ebx
		mov reg_copy_before[ 8 ], ecx
		mov reg_copy_before[ 12 ], edx

		cdq; ��������� ������� ����� eax � ����� �� 64 - ��� EAX : EDX
		div ebx; ������� ����� �� ebx

		mov reg_copy_after[ 0 ], eax
		mov reg_copy_after[ 4 ], ebx
		mov reg_copy_after[ 8 ], ecx
		mov reg_copy_after[ 12 ], edx
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
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );		// ����������� ������
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


void print_data( HDC _hdc, int _xpad, int _ypad, char * _sprintf_buf, const UINT * _data ) {

	int_to_cstring( _data[ 0 ] );

	TextOut( _hdc, _xpad, _ypad, _sprintf_buf, wsprintf( _sprintf_buf, "eax: %s", char_buf ) );

	int_to_cstring( _data[ 1 ] );

	TextOut( _hdc, _xpad, _ypad + 20, _sprintf_buf, wsprintf( _sprintf_buf, "ebx: %s", char_buf ) );

	int_to_cstring( _data[ 2 ] );

	TextOut( _hdc, _xpad, _ypad + 40, _sprintf_buf, wsprintf( _sprintf_buf, "ecx: %s", char_buf ) );

	int_to_cstring( _data[ 3 ] );

	TextOut( _hdc, _xpad, _ypad + 60, _sprintf_buf, wsprintf( _sprintf_buf, "edx: %s", char_buf ) );

}


//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  ������� ���������. ��������� � ������������ ��� ���������, ���������� � ����������
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	char sprintf_buf[ 60 ];

	switch ( message )
	{
	case WM_CREATE: // ��������� �������� ��� �������� ����
		//do_stuff();
		break;

	case WM_PAINT:  // ������������ ����
		hdc = BeginPaint( hWnd, &ps );	// ������ ����������� �����
		GetClientRect( hWnd, &rt ); // ������� ���� ��� ���������

		subs();

		TextOut( hdc, 10, 10, sprintf_buf, wsprintf( sprintf_buf, "Substitution (eax - edx):") );

		print_data( hdc, 10, 40, sprintf_buf, reg_copy_before );

		print_data( hdc, 10, 130, sprintf_buf, reg_copy_after );

		divs();

		TextOut( hdc, 400, 10, sprintf_buf, wsprintf( sprintf_buf, "Division (eax:edx / ebx):" ) );

		print_data( hdc, 400, 40, sprintf_buf, reg_copy_before );

		print_data( hdc, 400, 130, sprintf_buf, reg_copy_after );
		

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