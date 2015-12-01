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

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field )
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
