#include "poly_lib.h"

#define _USE_MATH_DEFINES
#include <math.h>

/**********************************************************************************/

void InitRegularPoligon( POINT * _pPoints, const short _nOfPoints, const RECT * _pRect ) {
	const POINT center = {
		( _pRect->right + _pRect->left ) / 2,
		( _pRect->bottom + _pRect->top ) / 2
	};

	const SIZE rectHalfSize = {
		( _pRect->right - _pRect->left ) / 2,
		( _pRect->bottom - _pRect->top ) / 2
	};

	for ( int i = 0; i < _nOfPoints; ++i ) {
		_pPoints[ i ].x = round( center.x + rectHalfSize.cx * cos( 2 * M_PI * i / _nOfPoints - M_PI_2 ) );
		_pPoints[ i ].y = round( center.y + rectHalfSize.cy * sin( 2 * M_PI * i / _nOfPoints - M_PI_2 ) );
	}
}
