#include "support.hpp"

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "lab2: metrics";
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

void PrintMetrics( HWND & _hWnd, HDC & _hdc, const TEXTMETRIC & _tm ) {
	// Область окна для рисования
	RECT clientRect;
	GetClientRect( _hWnd, &clientRect );

	// Строковый буфер
	char buff[ 60 ];

	// Отступ между координатами двух строк (полуторный интервал)
	const short padding = IntRound( _tm.tmHeight * 1.5 );

	// Вертикальная координата, начальная позиция - половина высоты строки
	long currYPos = _tm.tmHeight >> 1;

	// Вывод размера иконки
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Icon size: %dx%dpx.", GetSystemMetrics( SM_CXICON ), GetSystemMetrics( SM_CYICON )));
	
	// Сдвиг координаты и...
	currYPos += padding;
	// ...печать размеров рамки
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Window border width: %dpx, height: %dpx.", GetSystemMetrics( SM_CXBORDER ), GetSystemMetrics( SM_CYBORDER )));
	
	// Переход на нижний край клиентской области
	currYPos = clientRect.bottom - 2 * padding;

	// Вывод разрешения основного монитора
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Resolution of the main monitor: %dx%dpx.", GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN )));
	
	// Сдвиг координаты и...
	currYPos += padding;
	// ...печать средней ширины символа
	TextOut( _hdc, 10, currYPos, buff, wsprintf( buff, "Average char width: %d, line spacing: %d.", _tm.tmAveCharWidth, _tm.tmExternalLeading ) );

	/* О полях структуры tm:
	 * tmExternalLeading - минимальный межстрочный интервал, рекомендуемый разработчиком шрифта;
	 * tmInternalLeading - размер выступающих элементов букв,
	 * tmHeight - высота букв с учетом выступающих элементов.
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

	// Область окна для рисования
	GetClientRect( _hWnd, &clientRect );

	// Размеры окна
	GetWindowRect( _hWnd, &windowRect );

	// объявление описателей кисти
	HBRUSH hRedBrush, hGreenBrush, hOldBrush;

	// Создание кистей
	hRedBrush   = CreateSolidBrush( RGB( 255, 0, 0 ) );
	hGreenBrush = CreateSolidBrush( RGB( 0, 255, 0 ) );

	// Выбор кисти в контекст
	hOldBrush = (HBRUSH)SelectObject( _hdc, hRedBrush ); 

	// Диагонали вписанного эллипса
	SIZE ellipseSize {
		IntRound( ( windowRect.right - windowRect.left ) * 250.0 / wndDefaultSize.cx ),
		IntRound( ( windowRect.bottom - windowRect.top ) * 100.0 / wndDefaultSize.cy ),
	};

	// Позиция вписанного эллипса
	POINT ellipsePos = GetCenternedPosition( ellipseSize, clientRect );

	// Координаты описанного вокург эллипса прямоугольника
	//left top right bottom
	RECT epsDimensions {
		ellipsePos.x,
		ellipsePos.y,
		ellipsePos.x + ellipseSize.cx,
		ellipsePos.y + ellipseSize.cy
	};

	// Рисуем пятиугольник
	const short nOfPolygonPoints = 5;

	// Вычисляем размеры описанного эллипса
	RECT outerEllipse = GetOuterEllipse( epsDimensions, nOfPolygonPoints );

	// Рисуем полигон (пятиугольник, см. стр. 159)
	DrawRegularConvexPoligon( _hdc, nOfPolygonPoints, outerEllipse );

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
	// Сообщение приходит при создании окна
	case WM_CREATE: 
		hdc = BeginPaint( hWnd, &ps );
		GetTextMetrics( hdc, &tm );
		EndPaint( hWnd, &ps );
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		// Вывод информации
		PrintMetrics( hWnd, hdc, tm );
		PrintPolygons( hWnd, hdc );

		// Закончить графический вывод
		EndPaint( hWnd, &ps );
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