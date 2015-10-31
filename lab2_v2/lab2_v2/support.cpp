#include "support.hpp"
#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>

int IntRound( double _value ) {
	if ( _value >= 0 )
		return _value + 0.5;
	else
		return _value - 0.5;
}

POINT && GetCenternedPosition( const SIZE & _object, const RECT & _field )
{
	SIZE fieldSize{ 
		_field.right - _field.left, 
		_field.bottom - _field.top 
	};

	return POINT{
		_field.left + (( fieldSize.cx - _object.cx ) >> 1) ,
		_field.top  + (( fieldSize.cy - _object.cy ) >> 1)
	};
}

// TODO: Refactoring
void InitRegularPoligon( POINT * _pPoints, const short _nOfPoints, const RECT & _ellipse, EllipseType _ellipseType ) {
	POINT center{
		( _ellipse.right + _ellipse.left ) / 2,
		( _ellipse.bottom + _ellipse.top ) / 2
	};
	SIZE epsHalfSize;

	switch ( _ellipseType ) {
	case INNER:
		epsHalfSize = {
			IntRound(( _ellipse.right - _ellipse.left ) / (2 * cos( M_PI / _nOfPoints ))),
			IntRound(( _ellipse.bottom - _ellipse.top ) / (2 * cos( M_PI / _nOfPoints )))
		};
		break;

	case OUTER:
		epsHalfSize = {
			( _ellipse.right - _ellipse.left ) / 2,
			( _ellipse.bottom - _ellipse.top ) / 2
		};
		break;

	default:
		assert( !"Unknown ellipse type" );
	}

	assert( epsHalfSize.cx >= 0 && epsHalfSize.cy >= 0 );

	for ( int i = 0; i < _nOfPoints; ++i ) {
		_pPoints[ i ].x = center.x + epsHalfSize.cx * cos( 2 * M_PI * i / _nOfPoints - M_PI_2 );
		_pPoints[ i ].y = center.y + epsHalfSize.cy * sin( 2 * M_PI * i / _nOfPoints - M_PI_2 );
	}
}
