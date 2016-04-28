#include <windows.h>         // ����������� ���������� � ��������� API
#include <assert.h>

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "lab 9. Assembler usage";

const int N_OF_BITS = 32; // ������ ����� �� ������� ������

int arr1[ 3 ][ 3 ] = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
int arr2[ 3 ][ 3 ] = { { 5, 8, 30 },{ 6, 10, 11 },{ 0, 3, 8 } };
int arr3[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr4[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr5[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
int arr6[ 3 ][ 3 ] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };

void MTransponse( int nrows, int ncols ) {
	__asm {
	
		mov ebx, 0; i = 0

		per_row:
			cmp ebx, nrows; if i == nrows
			je do_exit
			mov ecx, 0; j = 0

		per_col:
			cmp ecx, ncols; if j == ncols
			je per_row_end;

			mov eax, ebx; eax = i
			mul ncols; eax:edx = ebx * ncols = i * ncols

			add eax, ecx; eax = i * ncols + j
			mov esi, eax; esi = i * ncols + j

			mov eax, arr1[ esi * 4 ]; temp = src[ i * ncols + j ]

			push eax; copy eax (temp) to stack

			mov eax, ecx; eax = j
			mul nrows; eax:ecx = ecx * nrows = j * nrows
			add eax, ebx; eax = j * nrows + i
			mov edi, eax; edi = j * nrows + i

			pop eax; restore eax (temp) from stack

			mov arr5[ edi * 4 ], eax; dest[ j * nrows + i ] = temp

			inc ecx; j++
			jmp per_col

		per_row_end:
			inc ebx; i++
			jmp per_row
		
		do_exit:
			; do nothing
	}

}

void MElementWiseMul( int nrows, int ncols ) {

	int n_of_elements = nrows * ncols;

	__asm {
			mov ecx, n_of_elements; comment
		r3 :
			mov eax, arr1[ ecx * 4 - 4 ]; ��������� ������� ������� arg1
			mov edx, arr2[ ecx * 4 - 4 ]; ��������� ������� ������� arg2
			mul edx; ��������� edx � aex
			mov arr3[ ecx * 4 - 4 ], eax; ����������� ���������� � ����� ������
			loop r3
	}

}

void MSum( int nrows, int ncols ) {

	int n_of_elements = nrows * ncols;

	__asm {
			mov ecx, n_of_elements; comment

		r1 :
			mov eax, arr1[ ecx * 4 - 4 ]; ��������� ������� ������� arg1
			mov edx, arr2[ ecx * 4 - 4 ]; ��������� ������� ������� arg2
			add eax, edx; �������� ��������� ������� 1 � 2
			mov arr4[ ecx * 4 - 4 ], eax; ����������� ���������� � ������ ������
			loop r1
	}

}

void MMul( int n, int m, int p ) {

	/*
		n - ���������� ����� ������ �������
		m - ���������� �������� ������ �������
		m - ���������� ����� ������ �������
		p - ���������� �������� ������ �������
	*/

	int i, j, k;

	__asm {
		; ebx is a value of element in dest matrix
		; eax is temp value
		; esi is for source array adressing
		; edi is for dest array adressing
		; ecx is a value in dest array, sum of products

		jmp start;

		start:
			mov i, 0; i = 0

		outer_loop:
			mov eax, i; eax = i
			cmp eax, n; if i == n
			je do_exit; then exit
			mov j, 0; else j = 0 and jmp middle_loop

		middle_loop:
			mov eax, j; eax = j
			cmp eax, p; if j == p
			je outer_loop_end; then exit middle_loop

			mov k, 0; else k = 0
			mov ecx, 0; and ecx = sum = 0 and jmp inner_loop

		inner_loop:
			mov eax, k; eax = k
			cmp eax, m; if k == m
			je middle_loop_end; then exit inner loop

			; form adress for the first source array
			mov eax, i; eax = i
			mul m; eax = i * m
			add eax, k; eax = i * m + k
			mov esi, eax; esi = eax = i * m + k

			; read value from the first source array to ebx
			mov ebx, arr1[ esi * 4 ];

			; form adress for the second source array
			mov eax, k; eax = k
			mul p; eax = k * p
			add eax, j; eax = k * p + j
			mov esi, eax; esi = eax = k * p + j

			; read value from the second source array to eax
			mov eax, arr2[ esi * 4 ];

			; multiply elements from first and second matrices, save result to eax
			mul ebx;

			; add result to sum
			add ecx, eax

			; inner_loop_end
			inc k; k++
			jmp inner_loop; start new inner_loop iteration

		middle_loop_end:
			; form adress for the second source array
			mov eax, i; eax = i
			mul p; eax = i * p
			add eax, j; eax = i * p + j
			mov edi, eax; esi = eax = i * p + j

			; write sum to dest array
			mov arr6[ edi * 4 ], ecx;

			inc j; j++
			jmp middle_loop; start new middle_loop iteration

		outer_loop_end:
			inc i; i++
			jmp outer_loop; start new outer_loop iteration
		
		do_exit:
	}

}

void PrintArray3x3( HDC _hdc, const RECT & _client, int _array[ 3 ][ 3 ], int _padding_x, int _padding_y, char * _buf ) {

	for ( int row = 0; row < 3; ++row ) {
		TextOut( _hdc, _padding_x, 20 * row + _padding_y + 20, _buf, wsprintf( _buf, "%d %d %d", _array[ row ][ 0 ], _array[ row ][ 1 ], _array[ row ][ 2 ] ) );
	}

}

void PrintArrays( HDC _hdc, const RECT & _client ) {

	// ��������� �����
	char buff[ 60 ];

	int padding_y = 10;
	int padding_x = 10;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Source 1:" ) );
	PrintArray3x3( _hdc, _client, arr1, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Source 2:" ) );
	PrintArray3x3( _hdc, _client, arr2, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "EW Mul result:" ) );
	PrintArray3x3( _hdc, _client, arr3, padding_x, padding_y, buff );

	padding_x = 10;	
	padding_y += 110;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Sum result:" ) );
	PrintArray3x3( _hdc, _client, arr4, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Src 1 tranposed:" ) );
	PrintArray3x3( _hdc, _client, arr5, padding_x, padding_y, buff );

	padding_x += 140;

	TextOut( _hdc, padding_x, padding_y, buff, wsprintf( buff, "Src1 x Src2 = " ) );
	PrintArray3x3( _hdc, _client, arr6, padding_x, padding_y, buff );
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
		//do_stuff();
		MElementWiseMul( 3, 3 );
		MSum( 3, 3 );
		MTransponse( 3, 3 );
		MMul( 3, 3, 3 );
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