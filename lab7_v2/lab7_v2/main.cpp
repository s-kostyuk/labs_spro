#include <windows.h>	// подключение библиотеки с функциями API

#include "resource.h"	// ресурсный файл

#include "../painter/painter.h" // header-файл библиотеки

#include <assert.h> 
#include <windowsx.h>
#include <tchar.h>

/**********************************************************************************/

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
PTCHAR szWindowClass = _T("Kostyuk");
PTCHAR szTitle = _T("lab7_v2: Star painter");

/**********************************************************************************/

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

/**********************************************************************************/

// Основная программа 
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PTCHAR    lpCmdLine,
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

/**********************************************************************************/

//  Регистрация класса окна 
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
	wcex.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );		// определение меню
	wcex.lpszClassName = szWindowClass;	// имя класса
	wcex.hIconSm = NULL;

	return RegisterClassEx( &wcex ); // регистрация класса окна
}

/**********************************************************************************/

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

/**********************************************************************************/

//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	static RECT currObjectDim; // Размеры текущего объекта

	// Тип указателя на функцию, которая будет производить рисование объекта размером RECT в контексте HDC
	typedef void ( * PainterFunc )( HDC, const RECT * );

	// "Переменная" - указатель на текущую функцию рисования 
	static PainterFunc fPainter = PaintPentagon;

	// Обработка сообщений
	switch ( message )
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		break;

	case WM_LBUTTONDOWN: // Нажатие на левую кнопку мыши
		// Фиксируем координаты
		// Левый верхний угол останется фиксированным. По умолчанию считаем, что фигура будет иметь нулевой размер
		currObjectDim.left = currObjectDim.right = GET_X_LPARAM( lParam );
		currObjectDim.top = currObjectDim.bottom = GET_Y_LPARAM( lParam );

		// Захватываем перемещения мыши
		SetCapture( hWnd );

		break;

	case WM_MOUSEMOVE: // Передвижение курсора мыши
		// Если мы передвигаем мышь с зажатой левой кнопкой
		if ( wParam & MK_LBUTTON ) {
			// Запоминаем новую координату правого верхнего угла
			currObjectDim.right  = GET_X_LPARAM( lParam );
			currObjectDim.bottom = GET_Y_LPARAM( lParam );

			// Перерисовываем содержимое окна
			InvalidateRect( hWnd, NULL, TRUE );
		}
		break;

	case WM_LBUTTONUP: // "Отжатие" левой кнопки мыши
		// Освобождаем мышь
		ReleaseCapture();
		break;

	case WM_PAINT:  // Перерисовать окно
		hdc = BeginPaint( hWnd, &ps );	// Начать графический вывод
		GetClientRect( hWnd, &rt ); // Область окна для рисования

		assert( fPainter != NULL ); // Перестраховка: указатель на функцию fPainter не должен быть NULL

		fPainter( hdc, &currObjectDim ); // Вызов текущей функции рисования

		EndPaint( hWnd, &ps );	// Закончить графический вывод
		break;

	case WM_DESTROY: // Завершение работы
		PostQuitMessage( 0 );
		break;

	case WM_COMMAND: // Обработка сообщений от меню
		switch ( LOWORD( wParam ) ) {
		case ID_CHANGE_STAR:
			fPainter = Paint5PointStar; // Функция рисования - функция рисования звезды
			break;

		case ID_CHANGE_POLYGON:
			fPainter = PaintPentagon; // Функция рисования - функция рисования пятиугольника
			break;

		case ID_PAINT_STAR:
			PaintTestStar( hWnd ); // Рисуем тестовую звезду во время вызова
			break;
		}
		break;

	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}