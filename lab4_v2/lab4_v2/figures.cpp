#include "figures.hpp"

/*****************************************************************************/

void DrawObject( HDC _hdc, const FigureType _type, const RECT & _dimen ) {
	switch ( _type )
	{
	case FigureType::LINE:
		MoveToEx( _hdc, _dimen.left, _dimen.top, NULL );
		LineTo( _hdc, _dimen.right, _dimen.bottom );
		break;

	case FigureType::ELLIPSE:
		Ellipse( _hdc, _dimen.left, _dimen.top, _dimen.right, _dimen.bottom );
		break;

	case FigureType::RECTANGLE:
		Rectangle( _hdc, _dimen.left, _dimen.top, _dimen.right, _dimen.bottom );
		break;

	default:
		break;
	}
}

/*****************************************************************************/

void DrawSavedObjects( HDC _hdc, const std::vector< FigureInfo > & _saved ) {
	for ( const FigureInfo & _c : _saved ) {
		DrawObject( _hdc, _c.m_type, _c.m_dim );
	}
}

/*****************************************************************************/
