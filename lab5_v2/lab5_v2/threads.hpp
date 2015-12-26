#ifndef _THREADS_HPP_
#define _THREADS_HPP_

/*****************************************************************************/

#include <windows.h>

/*****************************************************************************/

// Структура, получаемая worker'ами
typedef struct
{
	HDC    m_hdc; // описатель контекста, в котором нужно проводить рисование
	RECT   m_clientRect; // размер клиентской области
	BOOL   m_bKill; // завершить ли поток
	HANDLE m_semaphore; // к-во доступных перерисовок
	HANDLE m_semAvailable; // Событие - семафор доступен
	INT    m_maxNOfWorkers; // Максимальное к-во потоков, рисующих окружности
	CRITICAL_SECTION m_drawBlocker; // Критическая секция, блокировка вывода в контекст
	CRITICAL_SECTION m_semBlocker; // Блокировка изменений семафора
	SYNCHRONIZATION_BARRIER m_drawFinished; // рисование окончено
}
PARAMS, *PPARAMS;

/*****************************************************************************/

// Структура, получаемая потоком обновления экрана
typedef struct 
{
	HWND m_hWnd; // Описатель родительского окна
	BOOL m_bKill; // завершить ли поток
}
INVALIDATORPAR, *PINVALIDATORPAR;

/*****************************************************************************/

void Invalidator( PVOID _pvoid ); // Поток обновления экрана

void AlertThreadCreatureFail(); // Предупреждение об ошибке создания потока

void AlertSemaphoreCreatureFail();  // Предупреждение об ошибке создания семафора

void GrowSemaphore( PVOID _pvoid ); // Поток, увеличивающий верхнюю планку семафора

void WalkCircleThread( PVOID _pvoid ); // Поток, перемещающий окружности

/*****************************************************************************/

#endif // _THREADS_HPP_
