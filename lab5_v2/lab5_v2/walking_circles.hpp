#ifndef _WALKING_CIRCLES_HPP_
#define _WALKING_CIRCLES_HPP_

#include <windows.h>

typedef struct
{
	HWND  m_hWnd;
	POINT m_startPoint;
	RECT  m_clientRect;
	BOOL  m_bKill;
}
PARAMS, *PPARAMS;

void WalkCircleThread( PVOID _pvoid );

#endif // _WALKING_CIRCLES_HPP_