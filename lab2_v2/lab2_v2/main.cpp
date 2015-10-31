#include "support.hpp"

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "lab2: metrics";
const SIZE wndSize { 600, 400 };

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

	RECT screenField{
		0, 0,
		GetSystemMetrics( SM_CXSCREEN ),
		GetSystemMetrics( SM_CYSCREEN )
	};

	const POINT wndPos = GetCenternedPosition( wndSize, screenField );

	hWnd = CreateWindow( szWindowClass, // имя класса окна
		szTitle,   // имя приложения
		WS_MINIMIZEBOX | WS_TILED | WS_SIZEBOX | WS_SYSMENU, // стиль окна
		wndPos.x,	// положение по Х
		wndPos.y, 	// положение по Y
		wndSize.cx,    // размер по Х
		wndSize.cy,    // размер по Y
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

void PrintMetrics( HWND & _hWnd, HDC & _hdc, const TEXTMETRIC & _tm ) {
	RECT clientRect;
	GetClientRect( _hWnd, &clientRect ); // Область окна для рисования

	char buff[ 60 ];

	const short padding = _tm.tmHeight * 1.5;

	long currYPos = _tm.tmHeight >> 1;
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Icon size: %dx%dpx.", GetSystemMetrics( SM_CXICON ), GetSystemMetrics( SM_CYICON )));
	currYPos += padding;
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Window border width: %dpx, height: %dpx.", GetSystemMetrics( SM_CXBORDER ), GetSystemMetrics( SM_CYBORDER )));
	
	currYPos = clientRect.bottom - 2 * padding;
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Resolution of the main monitor: %dx%dpx.", GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN )));
	currYPos += padding;
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Average char width: %d, line spacing: %d.", _tm.tmAveCharWidth, _tm.tmInternalLeading ) );
}

void PrintRegularConvexPoligon( HDC & _hdc, const short _nOfPoints, const RECT & _ellipse, EllipseType _ellipseType ) {
	POINT * pPoints = new POINT[ _nOfPoints ];

	InitRegularPoligon( pPoints, _nOfPoints, _ellipse, _ellipseType );

	Polygon( _hdc, pPoints, _nOfPoints );

	delete[] pPoints;
}

// TODO: Refactoring
void PrintPolygons( HWND & _hWnd, HDC & _hdc ) {
	RECT clientRect, windowRect;

	GetClientRect( _hWnd, &clientRect ); // Область окна для рисования
	GetWindowRect( _hWnd, &windowRect ); // Размеры окна

	HBRUSH hRedBrush, hGreenBrush, hOldBrush; // объявление описателей кисти

	// Создание кистей
	hRedBrush   = CreateSolidBrush( RGB( 255, 0, 0 ) );
	hGreenBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );

	hOldBrush = (HBRUSH)SelectObject( _hdc, hRedBrush ); // Выбор кисти в контекст

	SIZE ellipseSize {
		IntRound( ( windowRect.right - windowRect.left ) * 250.0 / wndSize.cx ),
		IntRound( ( windowRect.bottom - windowRect.top ) * 100.0 / wndSize.cy ),
	};

	// Позиция эллипса
	POINT ellipsePos = GetCenternedPosition( ellipseSize, clientRect );

	//left top right bottom
	RECT epsDimensions {
		ellipsePos.x,
		ellipsePos.y,
		ellipsePos.x + ellipseSize.cx,
		ellipsePos.y + ellipseSize.cy
	};

	// Рисуем полигон
	PrintRegularConvexPoligon( _hdc, 5, epsDimensions, INNER );

	// Меняем кисть
	SelectObject( _hdc, hGreenBrush );

	// Рисуем эллипс
	Ellipse( _hdc, epsDimensions.left, epsDimensions.top, epsDimensions.right, epsDimensions.bottom );

	// Выбираем старую кисть
	SelectObject( _hdc, hOldBrush );

	// Удаляем созданные кисти
	DeleteObject( hRedBrush );
	DeleteObject( hGreenBrush );
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	static TEXTMETRIC tm;

	switch ( message )
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		hdc = BeginPaint( hWnd, &ps );
		GetTextMetrics( hdc, &tm );
		EndPaint( hWnd, &ps );
		break;

	case WM_PAINT:  // Перерисовать окно
		hdc = BeginPaint( hWnd, &ps );	// Начать графический вывод
		//hdc = GetDC( hWnd );

		PrintMetrics( hWnd, hdc, tm );

		PrintPolygons( hWnd, hdc );

		EndPaint( hWnd, &ps );	// Закончить графический вывод
		//ReleaseDC( hWnd, hdc );
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