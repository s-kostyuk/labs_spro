#include <windows.h>         // подключение библиотеки с функциями API
#include <time.h>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "ПЕРВАЯ ПРОГРАММА";

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
			mov eax, arr1[ ecx * 4 - 4 ]; последний элемент массива arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; последний элемент массива arr2
			add eax, ebx; сложение элементов массива 1 и 2
			mov arr3[ ecx * 4 - 4 ], eax; перемещение результата в третий массив
			loop r1
			
			; --------------------------------------
			
			mov ecx, 9; comment
		r2 :
			mov eax, arr1[ ecx * 4 - 4 ]; последний элемент массива arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; последний элемент массива arr2
			sub eax, ebx; разница элементов массива 1 и 2
			mov arr4[ ecx * 4 - 4 ], eax; перемещение результата в четвертый массив
			loop r2
			
			; -------------------------------------- 
			
			mov ecx, 9; comment
		r3 :
			mov eax, arr1[ ecx * 4 - 4 ]; последний элемент массива arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; последний элемент массива arr2
			mul ebx; умножение ebx и aex
			mov arr5[ ecx * 4 - 4 ], eax; перемещение результата в пятый массив
			loop r3

			; --------------------------------------

			mov ecx, 9; comment
		r4 :
			mov eax, arr1[ ecx * 4 - 4 ]; последний элемент массива arr1
			mov ebx, arr2[ ecx * 4 - 4 ]; последний элемент массива arr2
			div bl;
			mov arr6[ ecx * 4 - 4 ], al; перемещение результата в шестой массив
			mov arr7[ ecx * 4 - 4 ], ah; перемещение результата в седьмой массив
			loop r4
	}
}

// Предварительное описание функций

ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

// Основная программа 
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	MSG msg;

	// Регистрация класса окна 
	MyRegisterClass( hInstance );

	// Создание окна приложения
	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}

	// Цикл обработки сообщений
	while ( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  Регистрирует класс окна 

ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// стиль окна
	wcex.lpfnWndProc = (WNDPROC)WndProc; // оконная процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;		// указатель приложения
	wcex.hIcon = LoadIcon( NULL, IDI_HAND );		// определение иконки
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );    // определение курсора
	wcex.hbrBackground = GetSysColorBrush( COLOR_WINDOW );   // установка фона
	wcex.lpszMenuName = NULL;		// определение меню
	wcex.lpszClassName = szWindowClass;	// имя класса
	wcex.hIconSm = NULL;

	return RegisterClassEx( &wcex ); // регистрация класса окна
}


// FUNCTION: InitInstance(HANDLE, int)
// Создает окно приложения и сохраняет указатель приложения в переменной hInst
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWnd;

	hInst = hInstance; // сохраняет указатель приложения в переменной hInst

	hWnd = CreateWindow( szWindowClass, // имя класса окна
		szTitle,   // имя приложения
		WS_OVERLAPPEDWINDOW, // стиль окна
		CW_USEDEFAULT,	// положение по Х
		CW_USEDEFAULT, 	// положение по Y
		CW_USEDEFAULT,    // размер по Х
		CW_USEDEFAULT,    // размер по Y
		NULL,	// описатель родительского окна
		NULL,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL );     // параметры создания.

	if ( !hWnd ) // Если окно не создалось, функция возвращает FALSE
	{
		return FALSE;
	}
	ShowWindow( hWnd, nCmdShow );		// Показать окно
	UpdateWindow( hWnd );			// Обновить окно
	return TRUE;				//Успешное завершение функции
}

void PrintArray( HDC _hdc, const RECT & _client, int _array[3][3], int _padding_x, int _padding_y, char * _buf ) {

	for ( int row = 0; row < 3; ++row ) {
		TextOut( _hdc, _padding_x, 20 * row + _padding_y + 20, _buf, wsprintf( _buf, "%d %d %d", _array[ row ][0], _array[ row ][ 1 ], _array[ row ][ 2 ] ) );
	}

}

void PrintArrays( HDC _hdc, const RECT & _client ) {

	// Строковый буфер
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
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch ( message )
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		do_stuff();
		break;

	case WM_PAINT:  // Перерисовать окно
		hdc = BeginPaint( hWnd, &ps );	// Начать графический вывод
		GetClientRect( hWnd, &rt ); // Область окна для рисования

		PrintArrays( hdc, rt );

		EndPaint( hWnd, &ps );	// Закончить графический вывод
		break;

	case WM_DESTROY: // Завершение работы
		PostQuitMessage( 0 );
		break;
	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}