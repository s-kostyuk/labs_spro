#include <windows.h>         // подключение библиотеки с функциями API

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "Intro to assembler";

const int N_OF_BITS = 32; // Размер числа по условию задачи
CHAR char_buf[ N_OF_BITS + 1 ]; // Массив-буфер для хранения текстового представления числа
UINT32 reg_copy_before[ 4 ];
UINT32 reg_copy_after[ 4 ];

void int_to_cstring(int _num) {

	__asm {
		jmp global_start

		append_zero :
			mov char_buf[ ecx - 1 ], '0'; Дописываем значение
			jmp after_append

		append_one :
			mov	char_buf[ ecx - 1 ], '1'; Дописываем значение
			jmp after_append

		global_start :
			mov ecx, N_OF_BITS; заносим в счетчик количество битов для обработки
			mov	char_buf[ ecx ], 0; добавляем терминальный ноль в конец массива
			mov eax, _num; Заносим исходное число в аккумулятор

		convert_it:
			mov edx, eax; Копируем число в аккумулятор edx
			and edx, 1; Проверяем маладший бит, результат побитового И записываем в edx
			cmp edx, 0; Сравниваем число с нулем
			je append_zero; Если был ноль в младшей позиции - дописываем ноль
			jne append_one; Если была единица - записываем единицу

		after_append : ; после записи знака...
			sar eax, 1; Сдвигаем число
			loop convert_it; Возвращаемся в начало цикла
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

		cdq; расширяет двойное слово eax в слово из 64 - бит EAX : EDX
		div ebx; деление числа на ebx

		mov reg_copy_after[ 0 ], eax
		mov reg_copy_after[ 4 ], ebx
		mov reg_copy_after[ 8 ], ecx
		mov reg_copy_after[ 12 ], edx
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
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );		// определение иконки
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
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	char sprintf_buf[ 60 ];

	switch ( message )
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		//do_stuff();
		break;

	case WM_PAINT:  // Перерисовать окно
		hdc = BeginPaint( hWnd, &ps );	// Начать графический вывод
		GetClientRect( hWnd, &rt ); // Область окна для рисования

		subs();

		TextOut( hdc, 10, 10, sprintf_buf, wsprintf( sprintf_buf, "Substitution (eax - edx):") );

		print_data( hdc, 10, 40, sprintf_buf, reg_copy_before );

		print_data( hdc, 10, 130, sprintf_buf, reg_copy_after );

		divs();

		TextOut( hdc, 400, 10, sprintf_buf, wsprintf( sprintf_buf, "Division (eax:edx / ebx):" ) );

		print_data( hdc, 400, 40, sprintf_buf, reg_copy_before );

		print_data( hdc, 400, 130, sprintf_buf, reg_copy_after );
		

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