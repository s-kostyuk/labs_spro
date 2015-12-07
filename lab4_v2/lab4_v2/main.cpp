#include "support.hpp"

#include <Tchar.h>
//#include <cassert>
//#include <cstring>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = _T( "Kostyuk" );
LPCTSTR szTitle = _T( "lab4: child control windows" );
const SIZE wndDefaultSize{ 600, 400 };

struct
{
	long style;
	TCHAR * text;
}
const button[] =
{
	BS_PUSHBUTTON, "LINE",
	BS_PUSHBUTTON, "ELLIPSE",
	BS_PUSHBUTTON, "RECTANGLE"
};

//assert( ! strcmp( button[ ObjectType::LINE ].text, _T( "LINE" ) ) );

const int nOfButtons = sizeof( button ) / sizeof( button[ 0 ] );

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
	return (int)msg.wParam;
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

	static std::vector< std::pair < ObjectType, RECT > > savedObjects;
	static RECT currentObjectSize;
	static ObjectType currentObject = ObjectType::LINE;

	static HWND hwndButton[ nOfButtons ];
	static int cxChar, cyChar;

	static TEXTMETRIC tm;

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE:
		hdc = GetDC( hWnd );
		SelectObject( hdc, GetStockObject( SYSTEM_FIXED_FONT ) );
			GetTextMetrics( hdc, &tm );
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC( hWnd, hdc );

		for ( int i = 0; i < nOfButtons; i++ )
			hwndButton[ i ] = CreateWindow( _T( "button" ), button[ i ].text,
				WS_CHILD | WS_VISIBLE | button[ i ].style,
				cxChar, cyChar *( 1 + 2 * i ),
				20 * cxChar, 7 * cyChar / 4,
				hWnd, (HMENU)i,
				( (LPCREATESTRUCT)lParam )->hInstance, NULL );
		break;

	// Изменение размеров окна
	//case WM_SIZE:
	//	break;

	case WM_LBUTTONDOWN:
		currentObjectSize.left = LOWORD( lParam );
		currentObjectSize.top  = HIWORD( lParam );
		break;

	case WM_MOUSEMOVE:
		if ( wParam & MK_LBUTTON ) {
			currentObjectSize.right = LOWORD( lParam );
			currentObjectSize.bottom = HIWORD( lParam );
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;

	case WM_LBUTTONUP:
		savedObjects.push_back( { currentObject, currentObjectSize } );
		currentObjectSize = { 0, 0, 0, 0 };
		break;

		// Перерисовать окно
	case WM_PAINT:
		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		// Вывод информации
		DrawSavedObjects( hdc, savedObjects );
		DrawObject( hdc, currentObjectSize, currentObject );

		// Закончить графический вывод
		EndPaint( hWnd, &ps );

		break;

	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_DELETE:
			savedObjects.clear();
			InvalidateRect( hWnd, NULL, TRUE );
			break;

		default:
			break;
		}
		break;

	case WM_CHAR:
		switch ( wParam )
		{
		case 'r':
			currentObject = ObjectType::RECTANGLE;
			break;
		case 'e':
			currentObject = ObjectType::ELLIPSE;
			break;
		case 'l':
			currentObject = ObjectType::LINE;
			break;
		default:
			break;
		}

		break;

	case WM_DRAWITEM:
	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case (int)ObjectType::RECTANGLE:
			currentObject = ObjectType::RECTANGLE;
			break;
		case (int)ObjectType::ELLIPSE:
			currentObject = ObjectType::ELLIPSE;
			break;
		case (int)ObjectType::LINE:
			currentObject = ObjectType::LINE;
			break;
		default:
			break;
		}
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