#include "drawing_area.hpp"

#include "figures.hpp"

#include <cassert>

/*****************************************************************************/

RECT GetOffsetRect( const RECT & _r, int _dx, int _dy ) {
	RECT result = _r;

	OffsetRect( &result, _dx, _dy );

	return std::move( result );
}

/*****************************************************************************/

DrawingArea::DrawingArea( HWND _hWnd )
	: m_hWnd( _hWnd ), m_startPoint( { 0, 0 } )
{
	//update_size();
}

/*****************************************************************************/

void DrawingArea::redraw() {
	HDC hdc = GetDC( m_hWnd );

	redraw_in( hdc );

	ReleaseDC( m_hWnd, hdc );
}

/*****************************************************************************/

void DrawingArea::redraw_in( HDC _hdc ) {
	for ( const FigureInfo & _c : m_figures ) 
		DrawObject( 
			_hdc, 
			_c.m_type,
			GetOffsetRect( _c.m_dim, m_startPoint.x, m_startPoint.y )
		);
}

/*****************************************************************************/

void DrawingArea::push_back( const FigureInfo & _figure ) {
	m_figures.push_back( _figure );
}

/*****************************************************************************/

void DrawingArea::pop_back() {
	m_figures.pop_back();
}

/*****************************************************************************/

void DrawingArea::try_pop_back() {
	if( ! m_figures.empty() )
		m_figures.pop_back();
}

/*****************************************************************************/

void DrawingArea::clear() {
	m_figures.clear();
}

/*****************************************************************************/
