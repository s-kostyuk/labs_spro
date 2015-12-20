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

inline void MoveSomeScroll( WPARAM _wParam, LONG & _targetOffset, const LONG & _clientSize, const LONG & _drawedSize ) {
	switch ( LOWORD( _wParam ) )
	{
	case SB_PAGEDOWN:
		_targetOffset += _clientSize;
		// fall through 

	case SB_LINEDOWN:
		_targetOffset = min( _drawedSize, _targetOffset + 16 );
		break;

	case SB_PAGEUP:
		_targetOffset -= _clientSize;
		// fall through 

	case SB_LINEUP:
		_targetOffset = max( 0, _targetOffset - 16 );
		break;

	case SB_TOP:
		_targetOffset = 0;
		break;

	case SB_BOTTOM:
		_targetOffset = _drawedSize;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		_targetOffset = HIWORD( _wParam );
		break;

	default:
		break;
	}
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
	m_offset = { 0, 0 };
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

SCROLLINFO DrawingArea::get_common_scroll_info() {
	SCROLLINFO result;

	result.cbSize = sizeof( SCROLLINFO );
	result.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
	result.nMin = 0;

	return result;
}

/*****************************************************************************/

void DrawingArea::place_h_scroll( SCROLLINFO & _prepInfo ) {
	// Обработка изменения ширины
	if ( m_clientSize.cx >= m_drawedSize.cx ) {
		m_offset.x = 0;

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

	_prepInfo.nPos = m_offset.x;
	_prepInfo.nPage = m_clientSize.cx;
	_prepInfo.nMax = m_drawedSize.cx;

	SetScrollInfo( m_hScroll, SB_CTL, &_prepInfo, FALSE );
}

/*****************************************************************************/

void DrawingArea::place_v_scroll( SCROLLINFO & _prepInfo ) {
	// Обработка изменения высоты
	if ( m_clientSize.cy >= m_drawedSize.cy ) {
		m_offset.y = 0;

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

	_prepInfo.nPos = m_offset.y;
	_prepInfo.nPage = m_clientSize.cy;
	_prepInfo.nMax = m_drawedSize.cy;

	SetScrollInfo( m_vScroll, SB_CTL, &_prepInfo, FALSE );
}

/*****************************************************************************/

void DrawingArea::place_scrolls() {
	// TODO: Макcимально устранить дупликацию кода в place_h_scroll и place_v_scroll

	SCROLLINFO info = get_common_scroll_info();

	place_h_scroll( info );
	place_v_scroll( info );
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
			GetOffsetRect( fi.m_dim, -m_offset.x, -m_offset.y )
			//fi.m_dim
		);

	//ScrollDC( _hdc, -m_startPoint.x, -m_startPoint.y, NULL, NULL, NULL, NULL );
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
	OffsetRect( &_figure.m_dim, m_offset.x, m_offset.y );

	m_figures.push_back( _figure );

	refresh_drawed_size();

	place_scrolls();
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

void DrawingArea::h_scroll_move( WPARAM _wParam ) {
	MoveSomeScroll( _wParam, m_offset.x, m_clientSize.cx, m_drawedSize.cx );

	place_h_scroll( get_common_scroll_info() );
}

/*****************************************************************************/

void DrawingArea::v_scroll_move( WPARAM _wParam ) {
	MoveSomeScroll( _wParam, m_offset.y, m_clientSize.cy, m_drawedSize.cy );

	place_v_scroll( get_common_scroll_info() );
}

/*****************************************************************************/
