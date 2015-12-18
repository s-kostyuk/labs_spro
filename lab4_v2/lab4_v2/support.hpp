#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

/*****************************************************************************/

#include <windows.h> // подключение библиотеки с функциями API

#include <vector>

/*****************************************************************************/

int IntRound( double _value );

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

bool HasZeroSize( const RECT & _r );

/*****************************************************************************/

inline bool HasZeroSize( const RECT & _r ) {
	return _r.bottom == _r.top || _r.left == _r.right;
}

/*****************************************************************************/

#endif //_SUPPORT_HPP_