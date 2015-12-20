#include "main.hpp"

#include <Tchar.h>
#include <cassert>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = _T( "Kostyuk" );
LPCTSTR szTitle = _T( "lab4: child control windows" );
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
		WS_OVERLAPPEDWINDOW, // стиль окна
		wndPos.x,	// положение по Х
		wndPos.y, 	// положение по Y
		wndDefaultSize.cx,    // размер по Х
		wndDefaultSize.cy,    // размер по Y
		NULL,	// описатель родительского окна
		NULL,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL // параметры создания.
	);     

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

	static DrawingArea savedObjects;
	static RECT currObjectDim;
	static FigureType currObjectType = FigureType::LINE;

	static RECT invalidatedRect;

	static BtnController buttons;

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE:
		buttons.CreateWindows( hWnd );
		savedObjects.create_scrolls( hWnd );
		break;

	// Изменение размеров окна
	case WM_SIZE:
		savedObjects.resize( { LOWORD( lParam ), HIWORD( lParam ) } );
		break;

	case WM_LBUTTONDOWN:
		// Фиксируем координаты
		// Левый верхний угол останется фиксированным. По умолчанию считаем, что фигура будет иметь нулевой размер
		currObjectDim.left = currObjectDim.right = LOWORD( lParam );
		currObjectDim.top = currObjectDim.bottom = HIWORD( lParam );
		
		// Захватываем перемещения мыши
		//SetCapture( hWnd );
		
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
			
			currObjectDim.right = LOWORD( lParam );
			currObjectDim.bottom = HIWORD( lParam );

			// TODO: Как-то отимизировать, чтобы не перерисовывать все каждый раз
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;

	case WM_LBUTTONUP:
		// Если отпущена левая кнопка мыши и при этом фигура имеет не нулевой размер - запоминаем размеры фигуры
		if ( currObjectDim.right != currObjectDim.left && ! HasZeroSize( currObjectDim) ) 
			savedObjects.push_back( { currObjectType, currObjectDim } );
		
		// Сбрасываем размеры фигуры
		memset( &currObjectDim, 0, sizeof( currObjectDim ) );
		//currObjectDim = { 0, 0, 0, 0 };

		// Освобождаем мышь
		//ReleaseCapture();
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		// Вывод информации
		savedObjects.redraw_in( hdc );
		DrawObject( hdc, currObjectType, currObjectDim );

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
			//if ( !savedObjects.empty() ) {
				// Перерисовываем только тот участок окна, над которым был удаляемый объект
				//InvalidateRect( hWnd, &savedObjects.rbegin()->m_dim, TRUE );
				InvalidateRect( hWnd, NULL, TRUE );

				// Удаляем последний нарисованный объект
				savedObjects.try_pop_back();
			//}
			break;
		/*
		case VK_F1:
			ScrollWindowEx( hWnd, 0, 20, NULL, NULL, NULL, NULL, SW_ERASE );
			break;

		case VK_F2:
			ScrollWindowEx( hWnd, 0, -20, NULL, NULL, NULL, NULL, SW_ERASE );
			break;
		*/

		default:
			break;
		}
		break;

	// TODO: Может тут лучше использовать горячие клавиши?
	case WM_CHAR:
		switch ( wParam )
		{
		case 'r':
			currObjectType = FigureType::RECTANGLE;
			break;

		case 'e':
			currObjectType = FigureType::ELLIPSE;
			break;

		case 'l':
			currObjectType = FigureType::LINE;
			break;

		default:
			
			break;
		}

		break;

	case WM_DRAWITEM:
	case WM_COMMAND:
		assert( buttons.HandleClick( message, wParam, lParam ) != BtnController::BTN_UNKNOWN );
		currObjectType = (FigureType)buttons.HandleClick( message, wParam, lParam );
		
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