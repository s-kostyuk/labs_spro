#include <windows.h>         // подключение библиотеки с функциями API
#include "resource1.h"

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "Simple app";

const int xWindowSize = 400;
const int yWindowSize = 250;

// Предварительное описание функций

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Основная программа 
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	// Регистрация класса окна 
	MyRegisterClass(hInstance);

	// Создание окна приложения
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  Регистрирует класс окна 

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_DBLCLKS;	// стиль окна
	wcex.lpfnWndProc = (WNDPROC)WndProc; // оконная процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;		// указатель приложения
	wcex.hIcon = LoadIcon(NULL, IDI_INFORMATION);		// определение иконки
	wcex.hCursor = LoadCursor(NULL, IDC_WAIT);    // определение курсора
	wcex.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );   // установка фона
	wcex.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );		// определение меню
	wcex.lpszClassName = szWindowClass;	// имя класса
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex); // регистрация класса окна
}

// FUNCTION: InitInstance(HANDLE, int)
// Создает окно приложения и сохраняет указатель приложения в переменной hInst

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	const int xScreen = GetSystemMetrics(SM_CXSCREEN);
	const int yScreen = GetSystemMetrics(SM_CYSCREEN);

	const int xWindowPosition = (xScreen - xWindowSize) / 2;
	const int yWindowPosition = (yScreen - yWindowSize) / 2;

	HWND hWnd;

	hInst = hInstance; // сохраняет указатель приложения в переменной hInst

	hWnd = CreateWindow(szWindowClass, // имя класса окна
		szTitle,   // имя приложения
		WS_VSCROLL | WS_MINIMIZEBOX | WS_MINIMIZE | WS_TILED | WS_SYSMENU | WS_SIZEBOX, // стиль окна
		xWindowPosition,	// положение по Х
		yWindowPosition, 	// положение по Y
		xWindowSize,    // размер по Х
		yWindowSize,    // размер по Y
		NULL,	// описатель родительского окна
		NULL,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL);     // параметры создания.

	if (!hWnd) // Если окно не создалось, функция возвращает FALSE
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_MINIMIZE);		// Показать окно
	UpdateWindow(hWnd);			// Обновить окно
	return TRUE;				//Успешное завершение функции
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		break;

	case WM_PAINT:  // Перерисовать окно
		hdc = BeginPaint(hWnd, &ps);	// Начать графический вывод
		GetClientRect(hWnd, &rt); // Область окна для рисования
		DrawText(hdc, "[Make a double click here]", -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);	// Закончить графический вывод
		break;

	case WM_DESTROY: // Завершение работы
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDBLCLK:
		MessageBox(hWnd, "Congratulations! You made double a click", "Greeting", MB_OK);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_ABOUT_ABOUT:
			MessageBox(hWnd, "Created by Sergey Kostyuk, gr. CE-14-3", "About", MB_OK);
			break;

		case ID_CHANGE_COURSOR1:
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)) );
			break;

		case ID_CHANGE_COURSOR2:
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR2)));
			break;

		case ID_CHANGE_CURSOR3:
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR3)));
			break;
		}
	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}