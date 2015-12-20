#ifndef _DRAWING_AREA_HPP_
#define _DRAWING_AREA_HPP_

/*****************************************************************************/

#include <windows.h>
#include <vector>

/*****************************************************************************/

// Форвардное объявление с инофрмацией о рисуемых фигурах, полное подключается в cpp-файле
struct FigureInfo;

/*****************************************************************************/

class DrawingArea {

public:

	DrawingArea();
	~DrawingArea() = default;

	void create_scrolls( HWND _hWnd, const UINT _startHMenuValue = 0 );

	void resize( const SIZE & _newClientSize );
	void h_scroll_move( WPARAM _wParam, LPARAM _lParam );
	void v_scroll_move( WPARAM _wParam, LPARAM _lParam );

	// FIXME: For delete
	//void redraw();

	void redraw_in( HDC _hdc );

	void push_back( FigureInfo _figure );
	void try_pop_back();
	void clear();

private:

	//HWND  m_hWnd;
	POINT m_startPoint;
	//RECT  m_clientArea;
	SIZE  m_clientSize;
	SIZE  m_drawedSize;
	std::vector< FigureInfo > m_figures;

	HWND m_hScroll;
	HWND m_vScroll;

	std::pair< USHORT, USHORT > m_systemScrollSize;

	void place_scrolls();
	void refresh_drawed_size();
	void pop_back();
};

/*****************************************************************************/

inline void DrawingArea::resize( const SIZE & _newClientSize ) {
	m_clientSize = _newClientSize;

	place_scrolls();
}

/*****************************************************************************/

#endif // _DRAWING_AREA_HPP_