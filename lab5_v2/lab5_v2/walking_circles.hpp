#ifndef _WALKING_CIRCLES_HPP_
#define _WALKING_CIRCLES_HPP_

#include <windows.h>

typedef struct
{
	HDC    m_hdc; // описатель контекста, в котором нужно проводить рисование
	RECT   m_clientRect; // размер клиентсокй области
	BOOL   m_bKill; // завершить ли поток
	HANDLE m_semaphore; // к-во доступных перерисовок
	CRITICAL_SECTION m_drawBlocker; // критическая секция - отрисовка в HDC
	SYNCHRONIZATION_BARRIER m_drawFinished; // рисование окончено
}
PARAMS, *PPARAMS;

void WalkCircleThread( PVOID _pvoid );

#endif // _WALKING_CIRCLES_HPP_