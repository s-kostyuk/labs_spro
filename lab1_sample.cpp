#include <windows.h>         // подключение библиотеки с функциями API

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "ПЕРВАЯ ПРОГРАММА";

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
	if (!InitInstance (hInstance, nCmdShow)) 
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
	wcex.style			= CS_HREDRAW | CS_VREDRAW;	// стиль окна
	wcex.lpfnWndProc	= (WNDPROC)WndProc; // оконная процедура
	wcex.cbClsExtra		= 0;						
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;		// указатель приложения
	wcex.hIcon			= LoadIcon(NULL, IDI_HAND);		// определение иконки
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);    // определение курсора
	wcex.hbrBackground	= GetSysColorBrush(COLOR_WINDOW);   // установка фона
	wcex.lpszMenuName	= NULL;		// определение меню
	wcex.lpszClassName	= szWindowClass;	// имя класса
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex); // регистрация класса окна
}

// FUNCTION: InitInstance(HANDLE, int)
// Создает окно приложения и сохраняет указатель приложения в переменной hInst

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // сохраняет указатель приложения в переменной hInst

   hWnd=CreateWindow(szWindowClass, // имя класса окна
 szTitle,   // имя приложения
 WS_OVERLAPPEDWINDOW, // стиль окна
 CW_USEDEFAULT,	// положение по Х
 CW_USEDEFAULT, 	// положение по Y
 CW_USEDEFAULT,    // размер по Х
 CW_USEDEFAULT,    // размер по Y
 NULL,	// описатель родительского окна
 NULL,       // описатель меню окна
 hInstance,  // указатель приложения
 NULL);     // параметры создания.

   if (!hWnd) // Если окно не создалось, функция возвращает FALSE
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);		// Показать окно
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
			DrawText(hdc, "Привет, мир!", -1, &rt, DT_SINGLELINE|DT_CENTER | DT_VCENTER);
			EndPaint(hWnd, &ps);	// Закончить графический вывод
			break;

		case WM_DESTROY: // Завершение работы
			 PostQuitMessage(0);
			break;
		default: 
// Обработка сообщений, которые не обработаны пользователем
			return DefWindowProc(hWnd, message, wParam, lParam); 
   }
return 0;
}

