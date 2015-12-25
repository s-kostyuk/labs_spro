#ifndef _THREADS_HPP_
#define _THREADS_HPP_

#include <windows.h>

typedef struct
{
	HDC    m_hdc; // описатель контекста, в котором нужно проводить рисование
	RECT   m_clientRect; // размер клиентсокй области
	BOOL   m_bKill; // завершить ли поток
	HANDLE m_semaphore; // к-во доступных перерисовок
	HANDLE m_semAvailable; // Событие - семофор доступен
	INT    m_maxNOfWorkers; // Максимальное к-во потоков, рисующих окружности
	CRITICAL_SECTION m_drawBlocker; // Критическая секция, блокировка вывода в контекст
	CRITICAL_SECTION m_semBlocker; // Блокировка изменений семафора
	SYNCHRONIZATION_BARRIER m_drawFinished; // рисование окончено
}
PARAMS, *PPARAMS;

typedef struct 
{
	HWND m_hWnd; // Описатель родитеслького окна
	BOOL m_bKill; // завершить ли поток
}
INVALIDATORPAR, *PINVALIDATORPAR;

void Invalidator( PVOID _pvoid );

void AlertThreadCreatureFail();

void AlertSemaphoreCreatureFail();

void GrowSemaphore( PVOID _pvoid );

void WalkCircleThread( PVOID _pvoid );

#endif // _THREADS_HPP_