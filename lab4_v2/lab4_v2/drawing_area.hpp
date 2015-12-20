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

	void push_back( FigureInfo _figure );
	void try_pop_back();
	void clear();

	void create_scrolls( HWND _hWnd, const UINT _startHMenuValue = 0 );

	void resize( const SIZE & _newClientSize );

	// FIXME: For delete
	//void redraw();

	void redraw_in( HDC _hdc );

	void h_scroll_move( WPARAM _wParam );
	void v_scroll_move( WPARAM _wParam );

private:

	//HWND  m_hWnd;
	POINT m_offset;
	//RECT  m_clientArea;
	SIZE  m_clientSize;
	SIZE  m_drawedSize;
	std::vector< FigureInfo > m_figures;

	HWND m_hScroll;
	HWND m_vScroll;

	std::pair< USHORT, USHORT > m_systemScrollSize;

	SCROLLINFO get_common_scroll_info();

	void place_h_scroll( SCROLLINFO & _prepInfo );
	void place_v_scroll( SCROLLINFO & _prepInfo );

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