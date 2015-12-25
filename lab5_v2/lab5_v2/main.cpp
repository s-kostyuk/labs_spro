#include "main.hpp"

#include "walking_circles.hpp"

#include <Tchar.h>
#include <cassert>
#include <windowsx.h>
#include <cstdlib>
#include <ctime>
#include <vector>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = _T( "Kostyuk" );
LPCTSTR szTitle = _T( "lab5: multithreading" );
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
	
	static DWORD threadID;
	static INT nOfThreads;

	const INT maxNOfThreads = 100;

	static PARAMS params; // Параметры, передаваемые в поток
	RECT & clientRect = params.m_clientRect; // Размер клиентской области
	HDC & hdc = params.m_hdc; // Описатель констекста устройства вывода
	LPCRITICAL_SECTION pDrawBlocker = &params.m_drawBlocker; // Критическая секция, блокировка вывода в контекст
	LPSYNCHRONIZATION_BARRIER pDrawFinished = &params.m_drawFinished; // Барьер синхронизации, ожидание окончания вывода
	HANDLE & semaphore = params.m_semaphore; // квота на отрисовки

	static std::vector< HANDLE > allThreads;
	HANDLE tempThread;

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE:
		// Делаем посев случайных значений
		srand( time( NULL ) );

		// Инициализируем критическую секцию
		InitializeCriticalSection( pDrawBlocker );

		// Инициализируем барьер для одного (главного) потока
		InitializeSynchronizationBarrier( pDrawFinished, 1, 1 );

		semaphore = CreateSemaphore( NULL, 0, maxNOfThreads, _T( "Draw quota" ) );

		if ( semaphore == NULL ) {
			// FIXME: Тут должно быть какое-то более логичное действие
			assert( !"Failed to create thread" );
		}
		break;

	// Изменение размеров окна
	case WM_SIZE:
		// Пересчитываем размер клиентской области
		clientRect.right  = LOWORD( lParam );
		clientRect.bottom = HIWORD( lParam );
		break;

	// Нажатие клавиши клавиатуры
	case WM_KEYDOWN:
		if ( maxNOfThreads < allThreads.size() ) {
			// FIXME: Тут должно быть какое-то более логичное действие
			assert( !"Too mush threads" );
		}

		// Создаем новый поток с перемещ. окружностью:
		tempThread = CreateThread(
			NULL, // параметры безопасности;
			0, // размер стека;
			(LPTHREAD_START_ROUTINE)WalkCircleThread, //указатель на функцию;
			&params, // передаваемые параметры;
			0, // создаем приостановленный поток
			&threadID // указатель на переменную, в которой сохранится ID потока.
		);

		if ( tempThread == NULL ) {
			// FIXME: Тут должно быть какое-то более логичное действие
			assert( !"Failed to create thread" );
		}
		else {
			allThreads.push_back( tempThread );
		}

		// Генерируем новые барьеры
		DeleteSynchronizationBarrier( pDrawFinished );
		InitializeSynchronizationBarrier( pDrawFinished, allThreads.size() + 1, 1 );

		// Генерируем новый семафор
		//CloseHandle( semaphore );
		//semaphore = CreateSemaphore( NULL, 0, allThreads.size(), _T( "Draw quota" ) );

		if ( semaphore == NULL ) {
			// FIXME: Тут должно быть какое-то более логичное действие
			assert( !"Failed to create thread" );
		}

		// Запускаем перерисовку
		InvalidateRect( hWnd, NULL, TRUE );
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Получаем описатель контекста
		hdc = BeginPaint( hWnd, &ps );

		ReleaseSemaphore( semaphore, allThreads.size(), NULL );

		// Ждем, пока все потоки закончат отрисовку
		EnterSynchronizationBarrier( pDrawFinished, NULL );

		// Закрываем контекст
		EndPaint( hWnd, &ps );

		break;

	// Завершение работы
	case WM_DESTROY: 
		// Завершаем потоки
		params.m_bKill = true;

		// Удаляем критическую секцию
		DeleteCriticalSection( pDrawBlocker );

		// Закрываем семафор
		CloseHandle( semaphore );

		// Удаляем барьер
		DeleteSynchronizationBarrier( pDrawFinished );

		// Закрываем описатели потоков
		for ( HANDLE & hThread : allThreads ) {
			CloseHandle( hThread );
		}

		// Оставляем сообщение внешнему миру
		PostQuitMessage( 0 );
		break;

	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}
