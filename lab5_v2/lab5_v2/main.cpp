#include "main.hpp"

/*****************************************************************************/

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
LPCTSTR szWindowClass = _T( "Kostyuk" );
LPCTSTR szTitle = _T( "lab5: multithreading" );
const SIZE wndDefaultSize{ 600, 400 };

/*****************************************************************************/

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

/*****************************************************************************/

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

/*****************************************************************************/

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

/*****************************************************************************/

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	
	static DWORD threadID;
	static INT nOfWorkers;

	static PARAMS params; // Параметры, передаваемые в поток
	RECT & clientRect = params.m_clientRect; // Размер клиентской области
	HDC & hdc = params.m_hdc; // Описатель констекста устройства вывода
	LPSYNCHRONIZATION_BARRIER pDrawFinished = &params.m_drawFinished; // Барьер синхронизации, ожидание окончания вывода
	LPCRITICAL_SECTION pDrawBlocker = &params.m_drawBlocker;
	HANDLE & semDrawQuota = params.m_semaphore; // квота на отрисовки
	INT & maxNOfWorkers = params.m_maxNOfWorkers;
	HANDLE & evSemAvailable = params.m_semAvailable;

	static INVALIDATORPAR invParams;

	static bool growNeeded;

	static std::vector< HANDLE > workers;
	HANDLE tempThread;

	switch ( message )
	{
	// Сообщение приходит при создании окна
	case WM_CREATE:
		// Устанавливаем максимальное к-во рабочих процессов
		maxNOfWorkers = 5;

		// Делаем посев случайных значений
		//srand( time( NULL ) );

		// Инициализируем критическую секцию
		InitializeCriticalSection( pDrawBlocker );
		InitializeCriticalSection( &params.m_semBlocker );

		// Инициализируем барьер для одного (главного) потока
		InitializeSynchronizationBarrier( pDrawFinished, 1, 1 );

		// Создаем квоту на к-во отрисовок
		semDrawQuota = CreateSemaphore( NULL, 0, maxNOfWorkers, NULL );

		if ( semDrawQuota == NULL )
			AlertSemaphoreCreatureFail();

		// Разрешаем доступ к семафору
		evSemAvailable = CreateEvent( NULL, TRUE, TRUE, _T( "Semaphore available" ) );
		
		// Запускаем процесс обновления содержимого
		invParams.m_hWnd = hWnd;
		invParams.m_bKill = false;

		tempThread = CreateThread(
			NULL, // параметры безопасности;
			0, // размер стека;
			(LPTHREAD_START_ROUTINE)Invalidator, //указатель на функцию;
			&invParams, // передаваемые параметры;
			0, // создаем приостановленный поток
			&threadID // указатель на переменную, в которой сохранится ID потока.
		);

		// Если создание потока было не удачным - ругаемся
		if ( tempThread == NULL )
			AlertThreadCreatureFail();

		break;

	// Изменение размеров окна
	case WM_SIZE:
		// Пересчитываем размер клиентской области
		clientRect.right  = LOWORD( lParam );
		clientRect.bottom = HIWORD( lParam );
		break;

	// Нажатие клавиши клавиатуры
	case WM_KEYDOWN:
		// Создаем рабочий поток, перемещающий окружность по окну:
		tempThread = CreateThread(
			NULL, // параметры безопасности;
			0, // размер стека;
			(LPTHREAD_START_ROUTINE)WalkCircleThread, //указатель на функцию;
			&params, // передаваемые параметры;
			0, // создаем приостановленный поток
			&threadID // указатель на переменную, в которой сохранится ID потока.
		);

		// Если создание потока было не удачным - ругаемся
		if ( tempThread == NULL ) 
			AlertThreadCreatureFail();
		else 
			workers.push_back( tempThread );

		// Удаялем старый барьер
		DeleteSynchronizationBarrier( pDrawFinished );

		// Если к-во потоков приближается к максимальному...
		if ( ( maxNOfWorkers - 2 ) < workers.size() ) {
			// ...увеличим макс. число семофора
			growNeeded = true;	
		}

		InitializeSynchronizationBarrier( pDrawFinished, workers.size() + 1, 1 );

		// Запускаем перерисовку
		InvalidateRect( hWnd, NULL, TRUE );
		break;

	// Перерисовать окно
	case WM_PAINT:
		// Получаем описатель контекста
		hdc = BeginPaint( hWnd, &ps );

		// Разрешаем рисовать
		ReleaseSemaphore( semDrawQuota, workers.size(), NULL );

		if ( growNeeded ) {
			tempThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)GrowSemaphore, &params, 0, &threadID );

			// Если создание потока было не удачным - ругаемся
			if ( tempThread == NULL )
				AlertThreadCreatureFail();

			WaitForSingleObject( tempThread, INFINITE );

			growNeeded = false;
		}

		// Ждем, пока все потоки закончат отрисовку
		EnterSynchronizationBarrier( pDrawFinished,
			SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY );

		// Закрываем контекст
		EndPaint( hWnd, &ps );

		break;

	// Завершение работы
	case WM_DESTROY: 
		// Завершаем потоки
		params.m_bKill = true;
		invParams.m_bKill = true;

		// Удаляем критическую секцию
		DeleteCriticalSection( pDrawBlocker );
		DeleteCriticalSection( &params.m_semBlocker );

		// Запрещаем доступ к семафору
		CloseHandle( evSemAvailable );

		// Закрываем семафор
		CloseHandle( semDrawQuota );

		// Удаляем барьер
		DeleteSynchronizationBarrier( pDrawFinished );

		// Закрываем описатели потоков
		for ( HANDLE & hThread : workers )
			CloseHandle( hThread );

		// Оставляем сообщение внешнему миру
		PostQuitMessage( 0 );
		break;

	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}
