﻿#include "main.hpp"

#include "walking_circles.hpp"

#include <Tchar.h>
#include <cassert>
#include <windowsx.h>
#include <cstdlib>
#include <ctime>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = _T( "Kostyuk" );
LPCTSTR szTitle = _T( "lab5: multithreading" );
const SIZE wndDefaultSize{ 600, 400 };

RECT clientRect;

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
	static PARAMS params = { hWnd, NULL, NULL, false };
	static DWORD threadID;

	switch ( message )
	{
		// Сообщение приходит при создании окна
	case WM_CREATE:
		srand( time( NULL ) );
		break;

		// Изменение размеров окна
	case WM_SIZE:
		clientRect.right  = LOWORD( lParam );
		clientRect.bottom = HIWORD( lParam );

		params.m_clientRect = clientRect;
		break;

		/*
	case WM_LBUTTONUP:
		

		params.m_startPoint.x = GET_X_LPARAM( lParam );
		params.m_startPoint.y = GET_Y_LPARAM( lParam );

		CreateThread(
			NULL, // параметры безопасности
			0, // размер стека
			(LPTHREAD_START_ROUTINE)WalkCircleThread, //указатель на функцию,
			&params, // передаваемые параметры
			0, &threadID
			);
		break;
		*/

	case WM_KEYDOWN:
		params.m_startPoint.x = rand() % ( params.m_clientRect.right  - 40 );
		params.m_startPoint.y = rand() % ( params.m_clientRect.bottom - 40 );

		CreateThread(
			NULL, // параметры безопасности
			0, // размер стека
			(LPTHREAD_START_ROUTINE)WalkCircleThread, //указатель на функцию,
			&params, // передаваемые параметры
			0, &threadID
		);

		// Перерисовать окно
		
	case WM_PAINT:
		// Начать графический вывод
		//hdc = BeginPaint( hWnd, &ps );

		// Закончить графический вывод
		//EndPaint( hWnd, &ps );

		ValidateRect( hWnd, NULL );
		break;
		

	case WM_DESTROY: // Завершение работы
		params.m_bKill = true;
		PostQuitMessage( 0 );
		break;

	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}
