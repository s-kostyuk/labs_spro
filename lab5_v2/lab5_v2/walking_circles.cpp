#include "walking_circles.hpp"
#include "support.hpp"

#include <cstdlib>
#include <ctime>
#include <cassert>

enum class Direction {
	DONT_MOVE = -1, LEFT, RIGHT, UP, DOWN, N_OF_DIRECTIONS
};

Direction GetRandDirection() {
	return  (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );
}

//PVOID pvoid

void DetermineDirection( Direction & _targetDir, const RECT & _figureRect, const RECT & _clientRect ) {
	if ( _figureRect.left < _clientRect.left )
		_targetDir = Direction::RIGHT;

	else if ( _figureRect.right > _clientRect.right )
		_targetDir = Direction::LEFT;

	else if ( _figureRect.top < _clientRect.top )
		_targetDir = Direction::DOWN;

	else if ( _figureRect.bottom > _clientRect.bottom )
		_targetDir = Direction::UP;

	//else {} // Оставить неизменной
}

void MoveInDirection( RECT & _targetFigure, const Direction _direction, const INT _step = 1 ) {
	switch ( _direction )
	{
	case Direction::LEFT:
		OffsetRect( &_targetFigure, -_step, 0 );
		break;

	case Direction::RIGHT:
		OffsetRect( &_targetFigure, +_step, 0 );
		break;

	case Direction::UP:
		OffsetRect( &_targetFigure, 0, -_step );
		break;

	case Direction::DOWN:
		OffsetRect( &_targetFigure, 0, +_step );

	default:
		break;
	}
}

//void WalkCircle( HWND _hWnd, const POINT _startPoint, const RECT & _clientRect ) {
void WalkCircleThread( PVOID _pvoid ) {
	srand( time( NULL ) );

	PPARAMS pParams = (PPARAMS)_pvoid;

	const SIZE circleSize = { 40, 40 };

	HDC hdc;

	POINT startPoint;

	startPoint = {
		rand() % ( pParams->m_clientRect.right  - circleSize.cx ),
		rand() % ( pParams->m_clientRect.bottom - circleSize.cy )
	};

	RECT circleDims = { 
		startPoint.x,
		startPoint.y,
		startPoint.x + circleSize.cx,
		startPoint.y + circleSize.cy
	}; // left top right bottom

	const UINT step = 1;

	Direction direction;

	direction = (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );

	// Проверяем на необходимосты выхода из потока
	while ( ! pParams->m_bKill )
	{
		// Определяем направление движения окружности
		DetermineDirection( direction, circleDims, pParams->m_clientRect );

		// Определяем след. позицию окружности
		MoveInDirection( circleDims, direction, step );

		// Если поток завершают...
		if ( pParams->m_bKill )
			break; // ...выходим из цикла без попытки блокировки

		// Получаем свою квоту на перерисовку
		WaitForSingleObject( pParams->m_semaphore, INFINITE );

		// Блокируем отрисовку в других потоках
		EnterCriticalSection( &pParams->m_drawBlocker );

		// Рисуем окружность
		Ellipse( pParams->m_hdc, circleDims );

		// Снимаем блокировку
		LeaveCriticalSection( &pParams->m_drawBlocker );

		// Ожидаем завершения отрисовки во всех остальных потоках
		EnterSynchronizationBarrier( &pParams->m_drawFinished, NULL );
	}
}