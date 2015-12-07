﻿#include "support.hpp"

#include <Tchar.h>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = _T("Kostyuk");
LPCTSTR szTitle = _T("lab3: input");
const SIZE wndDefaultSize{ 600, 400 };
HGDIOBJ defaultFont = GetStockObject( SYSTEM_FIXED_FONT );

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
	return (int) msg.wParam;
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

	static TEXTMETRIC tm;
	static SIZE charSize;
	static RECT clientRect;

	static StdStringType buffer;
	static int currCharPosition = 0;

	static std::vector< StringInfo > preparedText;

	int xPadding = 10;
	static int maxCharsInLine;

	static POINT caretPosition{ 0, 0 };
	static Direction caretMoveDirection;

	TCHAR debug[ 30 ];

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE:
		hdc = GetDC( hWnd );

		SelectObject( hdc, defaultFont );
		
		GetTextMetrics( hdc, &tm );

		charSize.cx = tm.tmAveCharWidth;
		charSize.cy = tm.tmHeight;

		ReleaseDC( hWnd, hdc );

		break;

	// Изменение размеров окна
	case WM_SIZE:
		GetClientRect( hWnd, &clientRect );
		maxCharsInLine = (int)( (clientRect.right - clientRect.left - (xPadding << 1)) / charSize.cx );
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Ставим каретку на место
		CaretWinPosSetter( caretPosition, charSize, xPadding );

		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		// Выбираем моноширинный шрифт
		SelectObject( hdc, defaultFont );

		// Вывод информации
		DrawSavedText( hdc, preparedText, charSize, xPadding );

		TextOut( hdc, xPadding, clientRect.bottom - 10, debug, wsprintf( debug, _T("%d %d"), currCharPosition, buffer.size() ) );

		// Закончить графический вывод
		EndPaint( hWnd, &ps );

		break;

	// Нажатие ЛКМ - фиксирование позиции курсора
	//case WM_LBUTTONUP:
		//break;

	case WM_SETFOCUS:
		// Широкая каретка (rewrite mode)
		//CreateCaret( hWnd, NULL, xPadding, charSize.cy );

		// Каретка-палка (insert mode)
		CreateCaret( hWnd, (HBITMAP)0, 0, charSize.cy );
		CaretWinPosSetter( caretPosition, charSize, xPadding );
		ShowCaret( hWnd );
		break;

	case WM_KILLFOCUS:
		HideCaret( hWnd );
		DestroyCaret();

	// Обработка нажатия системных клавиш
	case WM_KEYDOWN:
		switch ( wParam ) {
		case VK_DELETE:
			if ( currCharPosition < buffer.size() ) 
				buffer.erase( currCharPosition, 1 );
			
			PrepareText( buffer, preparedText, charSize, maxCharsInLine );
			InvalidateRect( hWnd, NULL, TRUE );
			break;

		case VK_LEFT:
			if ( currCharPosition > 0 ) {
				MoveCaret( caretPosition, Direction::LEFT, preparedText );
				//--currCharPosition;
				currCharPosition = GetCharNumberByPos( caretPosition, preparedText );
			}
			
			break;

		case VK_RIGHT:
			if ( currCharPosition < buffer.size() ) {
				MoveCaret( caretPosition, Direction::RIGHT, preparedText );
				//++currCharPosition;
				currCharPosition = GetCharNumberByPos( caretPosition, preparedText );
			}
			
			break;

		case VK_UP:
			MoveCaret( caretPosition, Direction::UP, preparedText );
			currCharPosition = GetCharNumberByPos( caretPosition, preparedText );

			break;

		case VK_DOWN:
			MoveCaret( caretPosition, Direction::DOWN, preparedText );
			currCharPosition = GetCharNumberByPos( caretPosition, preparedText );

			break;

		default:
			break;
		}

		InvalidateRect( hWnd, NULL, TRUE );
		CaretWinPosSetter( caretPosition, charSize, xPadding );

		break;

	case WM_CHAR:
		switch ( wParam )
		{
		// New line
		case '\r':
			buffer.insert( currCharPosition, 1, '\n' );
			++ currCharPosition; 
				
			caretMoveDirection = Direction::RIGHT;
			break;

		// Backspace
		case '\b':
			if ( currCharPosition > 0 ) {
				-- currCharPosition;
				buffer.erase( currCharPosition, 1 );
				caretMoveDirection = Direction::LEFT;
			}
			break;

		// Escape
		/*case '\x1B':
			
			break;
		*/

		// Other symbols
		default:
			buffer.insert( currCharPosition, 1, (TCHAR) wParam );
			++ currCharPosition;
			caretMoveDirection = Direction::RIGHT;
			break;
		}

		PrepareText( buffer, preparedText, charSize, maxCharsInLine );
		MoveCaret( caretPosition, caretMoveDirection, preparedText );
		InvalidateRect( hWnd, NULL, TRUE );
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