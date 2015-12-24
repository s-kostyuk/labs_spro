#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

/*****************************************************************************/

#include <windows.h> // подключение библиотеки с функциями API

//#include <vector>

/*****************************************************************************/

int IntRound( double _value );

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

bool HasZeroSize( const RECT & _r );

BOOL Ellipse( HDC _hdc, const RECT & _dimensions );

/*****************************************************************************/

inline bool HasZeroSize( const RECT & _r ) {
	return _r.bottom == _r.top || _r.left == _r.right;
}

inline BOOL Ellipse( HDC _hdc, const RECT & _dimensions ) {
	Ellipse( _hdc, _dimensions.left, _dimensions.top, _dimensions.right, _dimensions.bottom );
}

/*****************************************************************************/

#endif //_SUPPORT_HPP_