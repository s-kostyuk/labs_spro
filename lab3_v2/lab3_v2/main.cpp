#include "support.hpp"

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "lab3: input";
const SIZE wndDefaultSize{ 600, 400 };

// Основная программа 
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd )
{
	MSG msg;

	// Регистрация класса окна 
	MyRegisterClass( hInstance );

	// Создание окна приложения
	if ( !InitInstance( hInstance, nShowCmd ) )
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
	wcex.hIcon = LoadIcon( NULL, IDI_INFORMATION );		// определение иконки
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

	const RECT screenField{
		0, 0,
		GetSystemMetrics( SM_CXSCREEN ),
		GetSystemMetrics( SM_CYSCREEN )
	};

	const POINT wndPos = GetCenternedPosition( wndDefaultSize, screenField );

	hWnd = CreateWindow( szWindowClass, // имя класса окна
		szTitle,   // имя приложения
		WS_MINIMIZEBOX | WS_TILED | WS_SIZEBOX | WS_SYSMENU, // стиль окна
		wndPos.x,	// положение по Х
		wndPos.y, 	// положение по Y
		wndDefaultSize.cx,    // размер по Х
		wndDefaultSize.cy,    // размер по Y
		NULL,	// описатель родительского окна
		NULL,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL );     // параметры создания.

	if ( !hWnd ) // Если окно не создалось, функция возвращает FALSE
		return FALSE;

	ShowWindow( hWnd, nCmdShow );		// Показать окно
	UpdateWindow( hWnd );			// Обновить окно
	return TRUE;				//Успешное завершение функции
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	static POINT printOutCoordinates = { 0, 0 };

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE: 
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		char buff[ 60 ];

		// Вывод информации
		TextOut( hdc, printOutCoordinates.x, printOutCoordinates.y, buff, wsprintf( buff, "Current coordinates: %dx%d", printOutCoordinates.x, printOutCoordinates.y ) );

		// Закончить графический вывод
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY: // Завершение работы
		PostQuitMessage( 0 );
		break;

	// По отжатию одной из клавиш мыши сохраняем новые координаты
	case WM_LBUTTONUP: case WM_MBUTTONUP: case WM_RBUTTONUP:
		printOutCoordinates.x = LOWORD( lParam );
		printOutCoordinates.y = HIWORD( lParam );
		InvalidateRect( hWnd, NULL, TRUE );
		break;

	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}