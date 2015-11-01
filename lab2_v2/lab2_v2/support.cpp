#include "support.hpp"
#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>

int IntRound( double _value ) {
	// Если число больше нуля - добавляем к нему 0.5, 
	// иначе - отнимаем 0.5
	_value += ( _value >= 0 ) ? 0.5 : -0.5;

	return (int)(_value);
}

POINT && GetCenternedPosition( const SIZE & _object, const RECT & _field )
{
	const SIZE fieldSize{ 
		_field.right - _field.left, 
		_field.bottom - _field.top 
	};

	return POINT{
		_field.left + (( fieldSize.cx - _object.cx ) >> 1) ,
		_field.top  + (( fieldSize.cy - _object.cy ) >> 1)
	};
}

RECT && GetOuterEllipse( const RECT & _innerDim, const short _nOfPolygonPoints ) {
	const SIZE innerSize{
		_innerDim.right - _innerDim.left,
		_innerDim.bottom - _innerDim.top
	};

	const double corrMultiplier = 0.5 / cos( M_PI / _nOfPolygonPoints ) - 0.5;

	const SIZE correction{
		IntRound( innerSize.cx * corrMultiplier ),
		IntRound( innerSize.cy * corrMultiplier ),
	};

	//left top right bottom
	return RECT{
		_innerDim.left   - correction.cx,
		_innerDim.top    - correction.cy,
		_innerDim.right  + correction.cx,
		_innerDim.bottom + correction.cy
	};
}

void InitRegularPoligon( POINT * _pPoints, const short _nOfPoints, const RECT & _ellipse ) {
	const POINT center{
		( _ellipse.right + _ellipse.left ) / 2,
		( _ellipse.bottom + _ellipse.top ) / 2
	};

	const SIZE epsHalfSize {
		( _ellipse.right - _ellipse.left ) >> 1,
		( _ellipse.bottom - _ellipse.top ) >> 1
	};

	assert( epsHalfSize.cx >= 0 && epsHalfSize.cy >= 0 );

	for ( int i = 0; i < _nOfPoints; ++i ) {
		_pPoints[ i ].x = IntRound( center.x + epsHalfSize.cx * cos( 2 * M_PI * i / _nOfPoints - M_PI_2 ));
		_pPoints[ i ].y = IntRound( center.y + epsHalfSize.cy * sin( 2 * M_PI * i / _nOfPoints - M_PI_2 ));
	}
}
