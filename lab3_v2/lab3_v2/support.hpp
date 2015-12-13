#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

/*****************************************************************************************************/

#include <windows.h> // подключение библиотеки с функциями API
#include <string>
#include <vector>

/*****************************************************************************************************/

// Сокращения для часто используемых типов
typedef std::pair< LPCTSTR, SIZE_T > StringInfo;
typedef std::basic_string< TCHAR > StdStringType;

/*****************************************************************************************************/

enum class Direction {
	/*NONE = -1, */RESET, UP, DOWN, LEFT, RIGHT
};

/*****************************************************************************************************/

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

/*****************************************************************************************************/

int IntRound( double _value );
POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

/*****************************************************************************************************/

void SplitTextOnDisplayRows( 
	const StdStringType & _source,
	std::vector< StringInfo > & _outputText,
	const INT _maxChars
);

void DrawSavedText( 
	HDC _hdc, 
	const std::vector< StringInfo > & _outputText,
	const SIZE & _charSize,
	const INT _xPadding
);

SIZE GetCharSize( HDC _hdc );

INT GetMaxCharsInLine( HWND _hWnd, const UINT _xPadding, const UINT _charWidth );

void ChangeCaret( HWND _hWnd, const bool _isInsertMode, const SIZE & _charSize );

INT GetCharNumberByPos( const POINT & _caretPos, const std::vector< StringInfo > & _text );

void MoveCaret( POINT & _caretPos, const Direction _d, const std::vector< StringInfo > & _text );

void SeekCaret( POINT & _caretPos, const INT _nOfChar, const std::vector< StringInfo > & _text );

void CaretDisplayPosSetter( const POINT & _caretPos, const SIZE & _charSize, INT _xPadding );

void PlaceCaretByCursor( POINT & _caretPos, const POINT & _cursorPos, const SIZE & _charSize, const std::vector< StringInfo > & _text, const UINT _xPadding );

bool IsAvailableForInput( const UINT _rowIndex, const std::vector< StringInfo > & _text );

bool IsPointsToNewline( const POINT & _caretPos, const std::vector< StringInfo > & _text );

void NormalizeX( POINT & _caretPos, const std::vector< StringInfo > & _text );

/*****************************************************************************************************/

inline void CaretDisplayPosSetter( const POINT & _caretPos, const SIZE & _charSize, INT _xPadding ) {
	SetCaretPos( _caretPos.x * _charSize.cx + _xPadding, _caretPos.y * _charSize.cy );
}

inline bool IsPointsToNewline( const POINT & _caretPos, const std::vector< StringInfo > & _text ) {
	return _caretPos.x > 0 && _text.at( _caretPos.y ).first[ _caretPos.x - 1 ] == '\n';
}

/*****************************************************************************************************/

#endif //_SUPPORT_HPP_
