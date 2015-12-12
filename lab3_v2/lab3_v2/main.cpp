#include "support.hpp"

#include <Tchar.h>
#include <cassert>

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
		WS_OVERLAPPEDWINDOW, // стиль окна
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

	static SIZE charSize;
	
	static StdStringType buffer;
	static int currCharPosition = 0;

	static std::vector< StringInfo > splittedText;

	int xPadding = 10;
	static int maxCharsInLine;

	static POINT caretPosition{ 0, 0 };
	static Direction caretMoveDirection;
	static bool isInsertMode = true;

	//TCHAR debug[ 30 ];

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE:
		hdc = GetDC( hWnd );

		SelectObject( hdc, defaultFont );
		
		charSize = GetCharSize( hdc );

		ReleaseDC( hWnd, hdc );

		break;

	// Изменение размеров окна
	case WM_SIZE:
		maxCharsInLine = GetMaxCharsInLine( hWnd, xPadding, charSize.cx );
		SplitTextOnDisplayRows( buffer, splittedText, maxCharsInLine );
		
		SeekCaret( caretPosition, currCharPosition, splittedText );
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Ставим каретку на место
		CaretDisplayPosSetter( caretPosition, charSize, xPadding );

		// Начать графический вывод
		hdc = BeginPaint( hWnd, &ps );

		// Выбираем моноширинный шрифт
		SelectObject( hdc, defaultFont );

		// Вывод информации
		DrawSavedText( hdc, splittedText, charSize, xPadding );

		//TODO: Удалить строку ниже, нужна для дебага
		//TextOut( hdc, xPadding, clientRect.bottom - 10, debug, wsprintf( debug, _T("%d %d %d"), currCharPosition, buffer.size(), preparedText.size() ) );

		// Закончить графический вывод
		EndPaint( hWnd, &ps );

		break;

	// Нажатие ЛКМ - фиксирование позиции курсора
	//case WM_LBUTTONUP:
		//break;

	case WM_SETFOCUS:
		ChangeCaret( hWnd, isInsertMode, charSize );
		CaretDisplayPosSetter( caretPosition, charSize, xPadding );
		break;

	case WM_KILLFOCUS:
		HideCaret( hWnd );
		DestroyCaret();
		break;

	// Обработка нажатия системных клавиш
	case WM_KEYDOWN:
		switch ( wParam ) {
		case VK_DELETE:
			// Курсор остается в той же позиции, вне зависимости от режима ввода

			// Если каретка находится не после последнего символа - удаляем символ
			if ( currCharPosition < buffer.size() ) {
				buffer.erase( currCharPosition, 1 );
				SplitTextOnDisplayRows( buffer, splittedText, maxCharsInLine );
				InvalidateRect( hWnd, NULL, TRUE );
			}

			break;

		case VK_INSERT:
			isInsertMode = !isInsertMode;
			ChangeCaret( hWnd, isInsertMode, charSize );
			break;

		case VK_LEFT:
			if ( currCharPosition > 0 ) {
				MoveCaret( caretPosition, Direction::LEFT, splittedText );
				//--currCharPosition;
				currCharPosition = GetCharNumberByPos( caretPosition, splittedText );
			}
			assert( currCharPosition <= buffer.size() );
			break;

		case VK_RIGHT:
			if ( currCharPosition < buffer.size() ) {
				MoveCaret( caretPosition, Direction::RIGHT, splittedText );
				//++currCharPosition;
				currCharPosition = GetCharNumberByPos( caretPosition, splittedText );
			}
			assert( currCharPosition <= buffer.size() );
			break;

		case VK_UP:
			MoveCaret( caretPosition, Direction::UP, splittedText );
			currCharPosition = GetCharNumberByPos( caretPosition, splittedText );
			assert( currCharPosition <= buffer.size() );
			break;

		case VK_DOWN:
			// Реагируем на нажатие только если есть куда двигаться вниз
			// Причем последняя строка - всегда пустая и не доступная для ввода
			if ( caretPosition.y < ( (INT)splittedText.size() - 2 ) ) {
				MoveCaret( caretPosition, Direction::DOWN, splittedText );
				currCharPosition = GetCharNumberByPos( caretPosition, splittedText );
				assert( currCharPosition <= buffer.size() );
			}
			break;
		}

		CaretDisplayPosSetter( caretPosition, charSize, xPadding );

		// TODO: Убрать строку ниже, она нужна для дебага
		//InvalidateRect( hWnd, NULL, TRUE );

		break;

	case WM_CHAR:
		switch ( wParam )
		{
		// New line
		case '\r':
			buffer.insert( currCharPosition, 1, '\n' );
			//++ currCharPosition; 
				
			// Сдвиг курсора на след. строку, вне зависимости от режима ввода
			caretMoveDirection = Direction::RIGHT;
			break;

		// Backspace
		case '\b':
			if ( currCharPosition > 0 ) {
				//-- currCharPosition;
				buffer.erase( currCharPosition - 1, 1 );
				// Сдвиг курсора влево, вне зависимости от режима ввода
				caretMoveDirection = Direction::LEFT;
			}
			break;

		// Escape
		/*case '\x1B':
			
			break;
		*/

		// Other symbols
		default:
			if ( isInsertMode ) {
				// В режиме вставки - добавляем символ в позицию, след. за текущей
				buffer.insert( currCharPosition, 1, (TCHAR)wParam );
			}
			else {
				// В режиме замены - заменяем текущий символ

				// TODO: Место для вашего бага и левых индексов

				// TODO: Подпираем костылем:
				// Режим замены работает только когда мы не находимся в начале и конце какой-либой строки

				assert( caretPosition.x >= 0 && caretPosition.y >= 0 && caretPosition.y < splittedText.size() );

				// Если мы находимся не в конце какой-либо строки
				if ( currCharPosition < splittedText.at( caretPosition.y ).second ) {
					assert( currCharPosition < buffer.size() );

					// Заменяем символ
					buffer.at( currCharPosition ) = (TCHAR)wParam;
				}
				else {
					assert( currCharPosition <= buffer.size() );
					buffer.insert( currCharPosition, 1, (TCHAR)wParam );
				}
			}
			//++ currCharPosition;
			// Сдвиг курсора вправо вне зависимости от режима ввода	
			caretMoveDirection = Direction::RIGHT;
			break;
		}

		SplitTextOnDisplayRows( buffer, splittedText, maxCharsInLine );
		MoveCaret( caretPosition, caretMoveDirection, splittedText );
		currCharPosition = GetCharNumberByPos( caretPosition, splittedText );
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
