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

	PAINTSTRUCT ps;

	HDC hdc;

	RECT circleDims = { 
		pParams->m_startPoint.x, 
		pParams->m_startPoint.y, 
		pParams->m_startPoint.x + circleSize.cx, 
		pParams->m_startPoint.y + circleSize.cy 
	}; // left top right bottom

	const UINT step = 1;

	Direction direction;

	direction = (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );

	while ( ! pParams->m_bKill )
	{
		InvalidateRect( pParams->m_hWnd, &circleDims, TRUE );

		DetermineDirection( direction, circleDims, pParams->m_clientRect );

		MoveInDirection( circleDims, direction, step );

		hdc = BeginPaint( pParams->m_hWnd, &ps );

		Ellipse( hdc, circleDims );

		EndPaint( pParams->m_hWnd, &ps );

		Sleep( 10 );
	}
}