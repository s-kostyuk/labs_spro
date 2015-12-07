#include "support.hpp"

int IntRound( double _value ) {
	// Если число больше нуля - добавляем к нему 0.5, 
	// иначе - отнимаем 0.5
	_value += ( _value >= 0 ) ? 0.5 : -0.5;

	return (int)( _value );
}

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field )
{
	const SIZE fieldSize{
		_field.right - _field.left,
		_field.bottom - _field.top
	};

	return POINT{
		_field.left + ( ( fieldSize.cx - _object.cx ) >> 1 ) ,
		_field.top + ( ( fieldSize.cy - _object.cy ) >> 1 )
	};
}

void DrawObject( HDC _hdc, const RECT & _objectCoordinates, const ObjectType _type ) {
	switch ( _type )
	{
	case ObjectType::LINE:
		MoveToEx( _hdc, _objectCoordinates.left, _objectCoordinates.top, NULL );
		LineTo( _hdc, _objectCoordinates.right, _objectCoordinates.bottom );
		break;

	case ObjectType::ELLIPSE:
		Ellipse( _hdc, _objectCoordinates.left, _objectCoordinates.top, _objectCoordinates.right, _objectCoordinates.bottom );
		break;

	case ObjectType::RECTANGLE:
		Rectangle( _hdc, _objectCoordinates.left, _objectCoordinates.top, _objectCoordinates.right, _objectCoordinates.bottom );
		break;

	default:
		break;
	}
}

void DrawSavedObjects( HDC _hdc, const std::vector< std::pair < ObjectType, RECT > > & _saved ) {
	for ( const std::pair < ObjectType, RECT > & _c : _saved ) {
		DrawObject( _hdc, _c.second, _c.first );
	}
}
