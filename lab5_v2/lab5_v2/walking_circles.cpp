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

void DetermineDirection( Direction & _targetDir, const POINT & _currPoint, const SIZE & _figureSize, const RECT & _clientRect ) {
	if ( _currPoint.x < _clientRect.left )
		_targetDir = Direction::RIGHT;

	else if ( _currPoint.x + _figureSize.cx > _clientRect.right )
		_targetDir = Direction::LEFT;

	else if ( _currPoint.y < _clientRect.top )
		_targetDir = Direction::DOWN;

	else if ( _currPoint.y + _figureSize.cy > _clientRect.bottom )
		_targetDir = Direction::UP;

	//else {} // Оставить неизменной
}

void MoveInDirection( POINT & _targetPos, const Direction _direction, const UINT _step = 1 ) {
	switch ( _direction )
	{
	case Direction::LEFT:
		_targetPos.x -= _step;
		break;

	case Direction::RIGHT:
		_targetPos.x += _step;
		break;

	case Direction::UP:
		_targetPos.y -= _step;
		break;

	case Direction::DOWN:
		_targetPos.y += _step;

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

	const UINT step = 1;

	Direction direction;
	POINT circlePos = pParams->m_startPoint;

	direction = (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );

	RECT ellipseRect = { circlePos.x, circlePos.y, circlePos.x + circleSize.cx, circlePos.y + circleSize.cy };

	while ( ! pParams->m_bKill )
	{
		DetermineDirection( direction, circlePos, circleSize, pParams->m_clientRect );

		MoveInDirection( circlePos, direction );

		

		InvalidateRect( pParams->m_hWnd, &ellipseRect, TRUE );

		ellipseRect = { circlePos.x, circlePos.y, circlePos.x + circleSize.cx, circlePos.y + circleSize.cy };

		hdc = BeginPaint( pParams->m_hWnd, &ps );

		Ellipse( hdc, ellipseRect );

		EndPaint( pParams->m_hWnd, &ps );

		Sleep( 10 );
	}
}