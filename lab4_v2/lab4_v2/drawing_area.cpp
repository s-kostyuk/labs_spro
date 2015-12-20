#include "drawing_area.hpp"

#include "figures.hpp"

#include <cassert>
#include <Tchar.h>

/*****************************************************************************/

inline void HideScroll( HWND _scroll ) {
	SetWindowPos(
		_scroll, HWND_TOP, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_HIDEWINDOW
	);
}

/*****************************************************************************/

RECT GetNormalizedRect( const RECT & _r ) {
	RECT result = _r;

	if ( result.bottom < result.top )
		std::swap( result.bottom, result.top );

	if ( result.right < result.left )
		std::swap( result.right, result.left );

	return result;
}

/*****************************************************************************/

UINT GetDiff( INT _bigger, INT _smaller ) {
	if ( _bigger <= _smaller )
		return 0;

	return _bigger - _smaller;
}

/*****************************************************************************/

RECT GetOffsetRect( const RECT & _r, int _dx, int _dy ) {
	RECT result = _r;

	OffsetRect( &result, _dx, _dy );

	return std::move( result );
}

/*****************************************************************************/

DrawingArea::DrawingArea() {
	m_startPoint = { 0, 0 };
	m_drawedSize = { 0, 0 };

	m_systemScrollSize = {
		GetSystemMetrics( SM_CYHSCROLL ),
		GetSystemMetrics( SM_CXVSCROLL )
	};
}

/*****************************************************************************/

void DrawingArea::create_scrolls( HWND _hWnd, const UINT _startHMenuValue )
{
	//m_hWnd = _hWnd;

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr( _hWnd, GWLP_HINSTANCE );

	m_hScroll = CreateWindow( 
		_T( "scrollbar" ), // имя класса окна
		NULL, // имя окна
		WS_CHILD | WS_VISIBLE | SBS_HORZ, // стили окна (дочернее, видимое, горизонтальный скролл)
		0, 0, // позиция левого верхнего угла
		0, 0, // размеры окна
		_hWnd, // описатель родительского окна
		( HMENU )_startHMenuValue, // уникальный идентификатор дочернего окна
		hInstance, // описатель процесса
		NULL
	);

	m_vScroll = CreateWindow(
		_T( "scrollbar" ), // имя класса окна
		NULL, // имя окна
		WS_CHILD | WS_VISIBLE | SBS_VERT, // стили окна (дочернее, видимое, вертикальный скролл)
		0, 0, // позиция левого верхнего угла
		0, 0, // размеры окна
		_hWnd, // описатель родительского окна
		(HMENU)( _startHMenuValue + 1 ), // уникальный идентификатор дочернего окна
		hInstance, // описатель процесса
		NULL
	);
}

/*****************************************************************************/

void DrawingArea::place_scrolls() {
	// TODO: Маскимально устранить дупликацию кода

	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof( SCROLLINFO );
	scrollInfo.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
	scrollInfo.nMin = 0;

	// Обработка изменения ширины
	if ( m_clientSize.cx >= m_drawedSize.cx ) {
		m_startPoint.x = 0;

		HideScroll( m_hScroll );
	}
	else {
		SetWindowPos(
			m_hScroll,
			HWND_TOP,
			0,
			m_clientSize.cy - m_systemScrollSize.first,
			m_clientSize.cx - m_systemScrollSize.second,
			m_systemScrollSize.first,
			SWP_SHOWWINDOW
		);
	}

	scrollInfo.nPos = m_startPoint.x;
	scrollInfo.nPage = m_clientSize.cx;
	scrollInfo.nMax = m_drawedSize.cx;

	SetScrollInfo( m_hScroll, SB_CTL, &scrollInfo, FALSE );

	// Обработка изменения высоты
	if ( m_clientSize.cy >= m_drawedSize.cy ) {
		m_startPoint.y = 0;

		HideScroll( m_vScroll );
	}
	else {
		SetWindowPos(
			m_vScroll,
			HWND_TOP,
			m_clientSize.cx - m_systemScrollSize.second,
			0,
			m_systemScrollSize.second,
			m_clientSize.cy - m_systemScrollSize.first,
			SWP_SHOWWINDOW
		);
	}

	scrollInfo.nPos = m_startPoint.y;
	scrollInfo.nPage = m_clientSize.cy;
	scrollInfo.nMax = m_drawedSize.cy;

	SetScrollInfo( m_vScroll, SB_CTL, &scrollInfo, FALSE );
}

/*****************************************************************************/
/*
void DrawingArea::redraw() {
	HDC hdc = GetDC( m_hWnd );

	redraw_in( hdc );

	ReleaseDC( m_hWnd, hdc );
}
*/
/*****************************************************************************/

void DrawingArea::redraw_in( HDC _hdc ) {
	for ( const FigureInfo & fi : m_figures ) 
		DrawObject( 
			_hdc, 
			fi.m_type,
			GetOffsetRect( fi.m_dim, m_startPoint.x, m_startPoint.y )
		);

	ScrollDC( _hdc, m_startPoint.x, m_startPoint.y, NULL, NULL, NULL, NULL );
}

/*****************************************************************************/

void DrawingArea::refresh_drawed_size() {
	RECT newDrawed;
	memset( &newDrawed, 0, sizeof( newDrawed ) );

	for ( const FigureInfo & fi : m_figures )
		UnionRect( &newDrawed, &GetNormalizedRect( fi.m_dim ), &newDrawed );
	

	m_drawedSize = {
		newDrawed.right,
		newDrawed.bottom
	};
}

/*****************************************************************************/

void DrawingArea::push_back( FigureInfo _figure ) {
	OffsetRect( &_figure.m_dim, m_startPoint.x, m_startPoint.y );

	m_figures.push_back( _figure );

	refresh_drawed_size();
}

/*****************************************************************************/

void DrawingArea::pop_back() {
	m_figures.pop_back();

	refresh_drawed_size();

	place_scrolls();
}

/*****************************************************************************/

void DrawingArea::try_pop_back() {
	if( ! m_figures.empty() )
		pop_back();
}

/*****************************************************************************/

void DrawingArea::clear() {
	m_figures.clear();

	memset( &m_drawedSize, 0, sizeof( m_drawedSize ) );

	place_scrolls();
}

/*****************************************************************************/
