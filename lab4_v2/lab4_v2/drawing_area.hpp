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

	explicit DrawingArea( HWND _hWnd );
	~DrawingArea() = default;

	void update_size();
	void h_scroll_move( WPARAM _wParam, LPARAM _lParam );
	void v_scroll_move( WPARAM _wParam, LPARAM _lParam );

	// FIXME: For delete
	//void redraw();

	void redraw_in( HDC _hdc );

	void push_back( const FigureInfo & _figure );
	void try_pop_back();
	void clear();

private:

	HWND  m_hWnd;
	POINT m_startPoint;
	RECT  m_clientArea;
	std::vector< FigureInfo > m_figures;

	HWND m_hScroll;
	HWND m_vScroll;

	void pop_back();

	// FIXME: For delete
	void redraw();

};

/*****************************************************************************/

#endif // _DRAWING_AREA_HPP_