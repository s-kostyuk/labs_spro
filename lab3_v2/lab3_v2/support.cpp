#include "support.hpp"

#include <cassert>

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

void PrepareText( const StdStringType & _source, std::vector< StringInfo > & _outputText, const SIZE & _charSize, const INT _maxChars ) {
	const LPCTSTR & source = _source.c_str();

	// Очищаем целевой вектор
	_outputText.clear();

	// Указатель на строку, которая будет напечатана в данный момент
	LPCTSTR currLine;

	// Число символов в текущей строке
	SIZE_T currLineSize;

	// Заполняем вектор указателей на строки. Один элемент вектора отвечает за одну строку, отображенную на экране
	for ( int i = 0; i < _source.size(); ++i ) {
		// Начинаем анализировать очередную строку
		currLine = source + i;

		// Находим окончание очередной строки
		for ( currLineSize = 0; i < _source.size(); ++currLineSize, ++i ) {
			// Если нашли символ перевода строки (перевод, заданный пользователем)...
			if ( source[ i + 1 ] == '\n'  ) {
				//++i;
				++currLineSize;
				// Нашли конец очередной строки, выходим из внутреннего цикла
				break;
			}

			// Если строка уже не вмещается в окно...
			if ( currLineSize == _maxChars ) {
				// ..нашли конец очередной строки, выходим из внутреннего цикла
				break;
			}
		}

		// Сохраняем найденную строку
		_outputText.push_back( { currLine, currLineSize } );
	}

}

void DrawSavedText( HDC _hdc, const std::vector< StringInfo > & _outputText, const SIZE & _charSize, const INT _xPxPadding ) {
	for ( int i = 0; i < _outputText.size(); ++i ) {
		TextOut(
			_hdc,
			_xPxPadding,
			i * _charSize.cy,
			_outputText[ i ].first,
			_outputText[ i ].second
			);
	}
}

void CaretWinPosSetter( const POINT & _caretPos, const SIZE & _charSize, INT _xPadding ) {
	SetCaretPos( _caretPos.x * _charSize.cx + _xPadding, _caretPos.y * _charSize.cy );
}

INT GetCharNumberByPos( const POINT & _caretPos, const std::vector< StringInfo > & _text ) {
	INT nOfChar = 0;

	for ( int i = 0; i < _caretPos.y; i++ ) {
		nOfChar += _text[ i ].second;
	}

	nOfChar += _caretPos.x;

	return nOfChar;
}

void MoveCaret( POINT & _caretPos, Direction _d, const std::vector< StringInfo > & _text ) {
	// Если текст отсутствует - нам нечего тут делать
	if ( _text.empty() )
		return;

	switch ( _d )
	{
	case Direction::RESET:
		_caretPos.x = 0;
		_caretPos.y = 0;
		break;

	case Direction::UP:
		// Если есть куда двигаться вверх
		if ( _caretPos.y > 0 ) {
			// Двигаемся вверх на одну строку
			--_caretPos.y;

			// Если строка выше короче, чем запрошенная позиция
			if ( ( _caretPos.x + 1 ) > _text[ _caretPos.y ].second ) {
				// Смещаем каретку влево
				_caretPos.x = _text[ _caretPos.y ].second - 1;
			}
		}
		break;

	case Direction::DOWN:
		// Если есть куда двигаться вниз
		if ( _caretPos.y < (_text.size() - 1) ) {
			// Двигаемся вниз на одну строку
			++_caretPos.y;

			// Если строка ниже короче, чем запрошенная позиция
			if ( ( _caretPos.x + 1 ) > _text[ _caretPos.y ].second ) {
				// Смещаем каретку влево
				_caretPos.x = _text[ _caretPos.y ].second - 1;
			}
		}
		break;

	case Direction::LEFT:
		// Если есть куда двигаться влево
		if ( _caretPos.x > 0 ) {
			// Сдвигаемся влево
			--_caretPos.x;
		}
		// Иначе пробуем сдвинуться вверх
		else {
			if ( _caretPos.y > 0 ) {
				_caretPos.x = _text[ _caretPos.y - 1 ].second - 1;
			}
			MoveCaret( _caretPos, Direction::UP, _text );
		}

		break;

	case Direction::RIGHT:
		// Если есть куда двигаться вправо
		if ( (_caretPos.x + 1) < _text[ _caretPos.y ].second ) {
			// Сдвигаемся вправо
			++_caretPos.x;
		}
		// Иначе пробуем сдвинуться вниз
		else {
			MoveCaret( _caretPos, Direction::DOWN, _text );
			_caretPos.x = 0;
		}
		break;

	default:
		assert( !"Unknown direction" );
		break;
	}
}
