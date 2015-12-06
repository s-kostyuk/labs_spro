#include "support.hpp"
#include <cassert>

#include <string>

int IntRound( double _value ) {
	// Если число больше нуля - добавляем к нему 0.5, 
	// иначе - отнимаем 0.5
	_value += ( _value >= 0 ) ? 0.5 : -0.5;

	return (int)(_value);
}

POINT GetCenternedPosition( const SIZE & _object, const RECT & _field )
{
	const SIZE fieldSize{ 
		_field.right - _field.left, 
		_field.bottom - _field.top 
	};

	return POINT{
		_field.left + (( fieldSize.cx - _object.cx ) >> 1) ,
		_field.top  + (( fieldSize.cy - _object.cy ) >> 1)
	};
}

void DrawSavedText( HDC _hdc, LPCTSTR _str, size_t _nOfCharacters, const SIZE & _charSize, const RECT & _clientArea ) {
	PAINTSTRUCT ps;
	
	const int xPadding = _charSize.cx/*px*/;

	const int maxChars = (int)(( _clientArea.right - _clientArea.left - (xPadding << 1) ) / _charSize.cx );

	// Устанавливаем указатель на строку, которая будет напечатана в данный момент
	LPCTSTR currentLine = _str;

	// Устанавливаем число символов в текущей строке
	int currentLineSize = 1;

	// Устанавлиаем номер текущей строки
	int currentLineNumber = 0;

	for ( int i = 0; i < _nOfCharacters; ++i ) {
		if ( _str[ i ] == '\n'
			||
			currentLineSize == maxChars
			)
		{
			TextOut(
				_hdc,
				xPadding,
				currentLineNumber * _charSize.cy,
				currentLine,
				currentLineSize
				);

			currentLine = _str + i;
			currentLineSize = 1;
			++currentLineNumber;
		}
		else {
			++currentLineSize;
		}
	}

	TextOut(
		_hdc,
		xPadding,
		currentLineNumber * _charSize.cy,
		currentLine,
		currentLineSize
		);
}
