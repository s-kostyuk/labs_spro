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
		for ( currLineSize = 0; i < _source.size(); ++i ) {
			// Если нашли символ перевода строки (перевод, заданный пользователем)...
			if ( source[ i ] == '\n'/* && currLineSize > 0*/ ) {
				// Оставляем его след. строке
				//--i;
				
				// Сохраняем
				++currLineSize;

				// Игнорируем

				// Нашли конец очередной строки, выходим из внутреннего цикла
				break;
			}

			// Если строка уже не вмещается в окно...
			if ( currLineSize == _maxChars ) {
				// ..нашли конец очередной строки, выходим из внутреннего цикла
				break;
			}

			// Иначе запоминаем очередной символ
			++currLineSize;
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

void ChangeCaret( HWND _hWnd, bool _isInsertMode, SIZE & _charSize ) {
	// Скрываем старую каретку, если она существовала
	HideCaret( _hWnd );

	// Удаляем старую каретку, если она существовала
	DestroyCaret();
			
	// Каретка-палка (insert mode)
	if ( _isInsertMode )
		CreateCaret( _hWnd, (HBITMAP)0, 1, _charSize.cy );
			
	// Широкая каретка (rewrite mode)
	else 
		CreateCaret( _hWnd, NULL, _charSize.cx, _charSize.cy );

	// Возвращаем каретку на место
	ShowCaret( _hWnd );
}

void CaretWinPosSetter( const POINT & _caretPos, const SIZE & _charSize, INT _xPadding ) {
	SetCaretPos( _caretPos.x * _charSize.cx + _xPadding, _caretPos.y * _charSize.cy );
}

INT GetCharNumberByPos( const POINT & _caretPos, const std::vector< StringInfo > & _text ) {
	INT nOfChar = 0;

	for ( int i = 0; i < _caretPos.y /*&& i < _text.size()*/; i++ ) {
		nOfChar += _text[ i ].second;
	}

	nOfChar += _caretPos.x;

	return nOfChar;
}

void MoveCaret( POINT & _caretPos, Direction _d, const std::vector< StringInfo > & _text ) {
	// TODO: Если эта штука сработает - ошибку будет обнаружить сложнее
	// Если текст отсутствует - сбрасываем позицию от греха подальше
	if ( _text.empty() ) {
		_caretPos.x = 0;
		_caretPos.y = 0;
		return;
	}

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
			if ( _caretPos.x > _text[ _caretPos.y ].second ) {
				// Смещаем каретку влево
				_caretPos.x = _text[ _caretPos.y ].second/* - 1*/;
			}
		}
		break;

	case Direction::DOWN:
		// Если есть куда двигаться вниз
		if ( _caretPos.y < _text.size() ) {
			// Двигаемся вниз на одну строку
			++_caretPos.y;

			// Если строка ниже еще не существует
			if ( _caretPos.y >= _text.size() )
				_caretPos.x = 0;

			// Если строка ниже короче, чем запрошенная позиция
			else if ( _caretPos.x > _text[ _caretPos.y ].second ) {
				// Смещаем каретку влево
				_caretPos.x = _text[ _caretPos.y ].second/* - 1*/;
			}
		}
		break;

	case Direction::LEFT:
		// Если есть куда двигаться влево
		if ( _caretPos.x > 0 ) {
			// Сдвигаемся влево
			--_caretPos.x;

			// Если там, куда мы пришли, хранится символ перевода строки - двигаемся влево опять
			//if( _text[ _caretPos.y ].first[ _caretPos.x ] == '\n' )
			//	MoveCaret( _caretPos, Direction::LEFT, _text );
		}
		// Иначе пробуем сдвинуться вверх
		else {
			while ( _caretPos.y > _text.size() ) {
				--_caretPos.y;
			}

			if ( _caretPos.y > 0 ) {
				_caretPos.x = _text[ _caretPos.y - 1 ].second;
				// Если мы попали на перевод строки - уменьшаем позицию
				if ( _text[ _caretPos.y - 1 ].first[ _caretPos.x - 1 ] == '\n' ) {
					// Смещаем каретку влево
					//--_caretPos.x;
					MoveCaret( _caretPos, Direction::LEFT, _text );
				}
			}
			MoveCaret( _caretPos, Direction::UP, _text );
		}

		break;

	case Direction::RIGHT:
		// Если есть куда двигаться вправо
		if ( (_caretPos.x) < _text[ _caretPos.y ].second ) {
			// Сдвигаемся вправо
			++_caretPos.x;

			// Если там, куда мы пришли, хранится символ перевода строки - двигаемся вправо опять
			if ( _text[ _caretPos.y ].first[ _caretPos.x - 1 ] == '\n' )
				MoveCaret( _caretPos, Direction::RIGHT, _text );
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