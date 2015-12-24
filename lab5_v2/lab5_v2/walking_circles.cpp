#include "walking_circles.hpp"
#include "support.hpp"

#include <cstdlib>
#include <ctime>
#include <cassert>

enum class Axis {
	UNDEF = -1, X, Y, N_OF_AXES
};

enum class Direction {
	DONT_MOVE = -1, LEFT, RIGHT, UP, DOWN, N_OF_DIRECTIONS
};

Axis GetRandomAxis() {
	return (Axis)(rand() % (INT)Axis::N_OF_AXES);
}

Axis GetAxis( const Direction _direction ) {
	switch ( _direction )
	{
	case Direction::DONT_MOVE:
		return Axis::UNDEF;
		break;

	case Direction::LEFT:
	case Direction::RIGHT:
		return Axis::X;
		break;

	case Direction::UP:
	case Direction::DOWN:
		return Axis::Y;
		break;

	default:
		assert( !"Wrong direction" );
	}
}

Direction GetRandDirection( const Axis _preferredAxis ) {
	switch ( _preferredAxis )
	{
	case Axis::UNDEF:
		return (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );
		break;

	case Axis::X:
		return (Direction)( rand() % 2 );
		break;

	case Axis::Y:
		return (Direction)( rand() % 2 + 2 );
		break;

	default:
		assert( ! "Wrong axis" );
		break;
	}
}

//PVOID pvoid

void DetermineDirection( Direction & _targetDir, const POINT & _currPoint, const SIZE & _figureSize, const RECT & _clientRect, const Axis _preferredAxis = Axis::UNDEF ) {
	if ( _currPoint.x < _clientRect.left )
		_targetDir = Direction::RIGHT;

	else if ( _currPoint.x + _figureSize.cx > _clientRect.right )
		_targetDir = Direction::LEFT;

	else if ( _currPoint.y < _clientRect.top )
		_targetDir = Direction::DOWN;

	else if ( _currPoint.y + _figureSize.cy > _clientRect.bottom )
		_targetDir = Direction::UP;

	else if ( GetAxis(  _targetDir ) != _preferredAxis ) {
		_targetDir = GetRandDirection( _preferredAxis );
	}
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

	HDC hdc;

	const UINT step = 1;

	Direction direction;
	Axis preferredAxis = GetRandomAxis();

	direction = (Direction)( rand() % (int)Direction::N_OF_DIRECTIONS );

	POINT circlePos = pParams->m_startPoint;

	while ( ! pParams->m_bKill )
	{
		DetermineDirection( direction, circlePos, circleSize, pParams->m_clientRect, preferredAxis );

		MoveInDirection( circlePos, direction );

		hdc = GetDC( pParams->m_hWnd );

		Ellipse( hdc, circlePos.x, circlePos.y, circlePos.x + circleSize.cx, circlePos.y + circleSize.cy );

		ReleaseDC( pParams->m_hWnd, hdc );

		Sleep( 10 );
	}
}