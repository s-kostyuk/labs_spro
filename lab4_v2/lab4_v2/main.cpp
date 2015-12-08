#include "support.hpp"

#include <Tchar.h>
#include <cassert>
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
} const button[] =
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
	static RECT currObjectSize;
	static ObjectType currObjectType = ObjectType::LINE;

	static HWND hwndButton[ nOfButtons ];
	static int cxChar, cyChar;

	static RECT invalidatedRect;

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
		// Фиксируем координаты
		// Левый верхний угол останется фиксированным. По умолчанию считаем, что фигура будет иметь нулевой размер
		currObjectSize.left = currObjectSize.right = LOWORD( lParam );
		currObjectSize.top = currObjectSize.bottom = HIWORD( lParam );
		
		// Захватываем перемещения мыши
		SetCapture( hWnd );
		
		// Переключаем фокус на наше окно (например, с дочернего окна; кнопки)
		// Если не снять фокус с дочерноего окна - сообщения о нажатых клавишах
		// клавиатуры будут приходить в оконнную процедуру дочернего, а не родительского окна

		// TODO: Подумать о горячих клавишах
		SetFocus( hWnd );
		break;

	case WM_MOUSEMOVE:
		// Если мы передвигаем мышь с зажатой левой кнопкой
		if ( wParam & MK_LBUTTON ) {
			// Запоминаем новую координату правого верхнего угла
			
			currObjectSize.right = LOWORD( lParam );
			currObjectSize.bottom = HIWORD( lParam );

			// TODO: Как-то отимизировать, чтобы не перерисовывать все каждый раз
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;

	case WM_LBUTTONUP:
		// Если отпущена левая кнопка мыши и при этом фигура имеет не нулевой размер - запоминаем размеры фигуры
		if ( currObjectSize.right != currObjectSize.left && currObjectSize.bottom != currObjectSize.top ) 
			savedObjects.push_back( { currObjectType, currObjectSize } );
		
		// Сбрасываем размеры фигуры
		currObjectSize = { 0, 0, 0, 0 };

		// Освобождаем мышь
		ReleaseCapture();
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		// Вывод информации
		DrawSavedObjects( hdc, savedObjects );
		DrawObject( hdc, currObjectSize, currObjectType );

		// Закончить графический вывод
		EndPaint( hWnd, &ps );

		break;

	// TODO: Может тут лучше использовать горячие клавиши?
	case WM_KEYDOWN:
		switch ( wParam )
		{
		// Очищаем буфер по нажатию ESC
		case VK_ESCAPE:
			savedObjects.clear();
			InvalidateRect( hWnd, NULL, TRUE );
			break;

		// Удаляем последнюю нарисованную фигуру по нажатию DELETE
		case VK_DELETE:
			if ( !savedObjects.empty() ) {
				// Перерисовываем только тот участок окна, над которым был удаляемый объект
				InvalidateRect( hWnd, &savedObjects.rbegin()->second, TRUE );

				// Удаляем последний нарисованный объект
				savedObjects.pop_back();
			}
			break;

		default:
			break;
		}
		break;

	// TODO: Может тут лучше использовать горячие клавиши?
	case WM_CHAR:
		switch ( wParam )
		{
		case 'r':
			currObjectType = ObjectType::RECTANGLE;
			break;
		case 'e':
			currObjectType = ObjectType::ELLIPSE;
			break;
		case 'l':
			currObjectType = ObjectType::LINE;
			break;
		default:
			
			break;
		}

		break;

	case WM_DRAWITEM:
	case WM_COMMAND:
		switch ( LOWORD( wParam ) ) {
		case (int)ObjectType::RECTANGLE:
			currObjectType = ObjectType::RECTANGLE;
			break;

		case (int)ObjectType::ELLIPSE:
			currObjectType = ObjectType::ELLIPSE;
			break;

		case (int)ObjectType::LINE:
			currObjectType = ObjectType::LINE;
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