#include "btn_controller.hpp"
//#include "support.hpp"

#include <cassert>
#include <Tchar.h>

/*****************************************************************************/

void BtnController::InitButtons() {
	assert( N_OF_BUTTONS == m_buttons.size() );

	m_buttons.at( BTN_ELLIPSE )   = { _T(  "Ellipse"  ), BS_PUSHBUTTON };
	m_buttons.at( BTN_LINE )      = { _T(   "Line"    ), BS_PUSHBUTTON };
	m_buttons.at( BTN_RECTANGLE ) = { _T( "Rectangle" ), BS_PUSHBUTTON };
}

/*****************************************************************************/

void BtnController::CreateWindows( const HWND _parentWindow ) {
	SIZE charSize;
	TEXTMETRIC tm;

	HDC hdc = GetDC( _parentWindow );
	SelectObject( hdc, GetStockObject( SYSTEM_FIXED_FONT ) );
	GetTextMetrics( hdc, &tm );
	charSize.cx = tm.tmAveCharWidth;
	charSize.cy = tm.tmHeight + tm.tmExternalLeading;
	ReleaseDC( _parentWindow, hdc );

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr( _parentWindow, GWLP_HINSTANCE );

	for ( int i = 0; i < m_buttons.size(); i++ )
		m_buttons.at( i ).m_hWnd = 
		CreateWindow( 
			_T( "button" ), 
			m_buttons.at( i ).m_name.c_str(),
			WS_CHILD | WS_VISIBLE | m_buttons.at( i ).m_style,
			charSize.cx, 
			charSize.cy *( 1 + 2 * i ),
			20 * charSize.cx, 
			7 * charSize.cy / 4,
			_parentWindow, 
			(HMENU)i,
			hInstance, NULL 
		);
}

/*****************************************************************************/

BtnController::BtnController() {
	InitButtons();
}

/*****************************************************************************/

BtnController::ButtonID 
BtnController::HandleClick( const UINT _message, const WPARAM _wParam, const LPARAM _lParam ) 
{
	assert( _message == WM_DRAWITEM || _message == WM_COMMAND );

	if ( LOWORD( _wParam ) >= N_OF_BUTTONS )
		return BTN_UNKNOWN;
	
	/*
	switch ( LOWORD( _wParam ) ) {
	case (int)BtnController::BTN_RECTANGLE:
		currObjectType = ObjectType::RECTANGLE;
		break;

	case (int)BtnController::BTN_ELLIPSE:
		currObjectType = ObjectType::ELLIPSE;
		break;

	case (int)BtnController::BTN_LINE:
		currObjectType = ObjectType::LINE;
		break;

	default:
		assert( !"Unknown button" );
		break;
	}
	break;
	*/

	return (ButtonID)LOWORD( _wParam );
}

/*****************************************************************************/
