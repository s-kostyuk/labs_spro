#ifndef _SUPPORT_HPP_
#define _SUPPORT_HPP_

#include <windows.h> // подключение библиотеки с функциями API
#include <string>
#include <vector>

// Сокращения для часто используемых типов
typedef std::pair< LPCTSTR, SIZE_T > StringInfo;
typedef std::basic_string< TCHAR > StdStringType;

// Предварительное описание функций
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

int IntRound( double _value );
POINT GetCenternedPosition( const SIZE & _object, const RECT & _field );

void PrepareText( 
	const StdStringType & _source,
	std::vector< StringInfo > & _outputText,
	const SIZE & _charSize, 
	const INT _maxChars
);

void DrawSavedText( 
	HDC _hdc, 
	const std::vector< StringInfo > & _outputText,
	const SIZE & _charSize,
	const INT _xPadding
);

enum class Direction {
	RESET, UP, DOWN, LEFT, RIGHT
};

INT GetCharNumberByPos( const POINT & _caretPos, const std::vector< StringInfo > & _text );

void MoveCaret( POINT & _caretPos, Direction _d, const std::vector< StringInfo > & _text );

void CaretWinPosSetter( const POINT & _caretPos, const SIZE & _charSize, INT _xPadding );

#endif //_SUPPORT_HPP_