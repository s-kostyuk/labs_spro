#include "support.hpp"

#include <cassert>

/*****************************************************************************************************/

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

/*****************************************************************************************************/

SIZE GetCharSize( HDC _hdc ) {
	TEXTMETRIC tm;

	GetTextMetrics( _hdc, &tm );

	return SIZE{ tm.tmAveCharWidth, tm.tmHeight };
}

INT GetMaxCharsInLine( HWND _hWnd, const UINT _xPadding, const UINT _charWidth ) {
	RECT clientRect;

	GetClientRect( _hWnd, &clientRect );

	return (int)( ( clientRect.right - clientRect.left - ( _xPadding << 1 ) ) / _charWidth );
}

void SplitTextOnDisplayRows( const StdStringType & _source, std::vector< StringInfo > & _outputText, const INT _maxChars ) {
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
			if ( source[ i ] == '\n' ) {		
				// Сохраняем
				++currLineSize;

				// Нашли конец очередной строки, выходим из внутреннего цикла
				break;
			}

			// Если строка уже не вмещается в окно...
			if ( currLineSize == _maxChars ) {
				// ...оставляем символ след. строке...
				--i;

				// ...нашли конец очередной строки, выходим из внутреннего цикла
				break;
			}

			// Иначе запоминаем очередной символ
			++currLineSize;
		}

		// Сохраняем найденную строку
		_outputText.push_back( { currLine, currLineSize } );
	}

	// Добавляем одну пустую строку в конец вектора
	_outputText.push_back( { source + _source.size() - 1, 0 } );
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

void ChangeCaret( HWND _hWnd, const bool _isInsertMode, const SIZE & _charSize ) {
	// Скрываем старую каретку, если она существовала
	HideCaret( _hWnd );

	// Удаляем старую каретку, если она существовала
	DestroyCaret();
	
	// Устанавливаем ширину каретки:
	UINT caretWidth = 
		// В режиме вставки она равна одному пикселю
		( _isInsertMode ) ? 1 
		// Иначе (в режиме замены) - ширине символа
		: _charSize.cx;
			
	// Создаем каретку нужного размера
	CreateCaret( _hWnd, NULL, caretWidth, _charSize.cy );
	
	// Возвращаем каретку на место
	ShowCaret( _hWnd );
}

INT GetCharNumberByPos( const POINT & _caretPos, const std::vector< StringInfo > & _text ) {
	INT nOfChar = 0;

	for ( int i = 0; i < _caretPos.y; ++i ) {
		assert( i < _text.size() );
		nOfChar += _text.at( i ).second;
	}

	nOfChar += _caretPos.x;

	return nOfChar;
}

void MoveCaret( POINT & _caretPos, const Direction _d, const std::vector< StringInfo > & _text ) {
	// TODO: Если эта штука сработает - ошибку будет обнаружить сложнее
	// Если текст отсутствует - сбрасываем позицию от греха подальше
	if ( _text.empty() ) {
		_caretPos.x = 0;
		_caretPos.y = 0;
		return;
	}

	if ( _caretPos.y < _text.size() && _d != Direction::UP ) {
		assert( _caretPos.x <= _text.at( _caretPos.y ).second + 1 );
		assert( _d == Direction::LEFT || _caretPos.x <= _text.at( _caretPos.y ).second );
	}
	else {
		assert( _caretPos.y <= _text.size() && ( _d == Direction::LEFT || _d == Direction::UP ) );
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

			// Приводим позицию в строке к корректному состоянию
			NormalizeX( _caretPos, _text );
		}
		break;

	case Direction::DOWN:
		// Если есть куда двигаться вниз
		if ( _caretPos.y < ( (int)_text.size() - 1 ) ) {
			// Двигаемся вниз на одну строку
			++_caretPos.y;

			// Приводим позицию в строке к корректному состоянию
			NormalizeX( _caretPos, _text );
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
				// Перекидываем каретку в конец строки выше
				_caretPos.x = _text.at( _caretPos.y - 1 ).second;

				// Двигаемся вверх
				MoveCaret( _caretPos, Direction::UP, _text );
			}
		}
		assert( _caretPos.x <= _text.at( _caretPos.y ).second && _caretPos.x >= 0 );
		break;

	case Direction::RIGHT:
		// Если есть куда двигаться вправо
		if ( _caretPos.x < _text.at( _caretPos.y ).second ) {
			// Сдвигаемся вправо
			++_caretPos.x;

			// Если там, куда мы пришли, хранится символ перевода строки - двигаемся вправо опять
			if ( IsPointsToNewline( _caretPos, _text ) ) 
				MoveCaret( _caretPos, Direction::RIGHT, _text );
			
		}

		// Иначе пробуем сдвинуться вниз
		else {
			// Ставим каретку на начало след. строки
			_caretPos.x = 0;
			
			// Двигаемся вниз
			MoveCaret( _caretPos, Direction::DOWN, _text );
		}
		break;

	default:
		assert( !"Unknown direction" );
		break;
	}
}

void SeekCaret( POINT & _caretPos, const INT _nOfChar, const std::vector< StringInfo > & _text ) {
	_caretPos.x = 0;
	
	INT charsCounted = 0;

	LONG & iLine = _caretPos.y;

	for ( iLine = 0; iLine < _text.size(); ++iLine ) {
		charsCounted += _text.at( iLine ).second;

		if ( charsCounted >= _nOfChar ) {
			charsCounted -= _text.at( iLine ).second;
			break;
		}
	}

	_caretPos.x = _nOfChar - charsCounted;
}

void PlaceCaretByCursor( POINT & _caretPos, const POINT & _cursorPos, const SIZE & _charSize, const std::vector< StringInfo > & _text, const UINT _xPadding ) {
	// Ставим каретку в координату, соотв. указанной позиции в окне
	_caretPos.x = IntRound( (DOUBLE)(_cursorPos.x - _xPadding ) / _charSize.cx );
	_caretPos.y = _cursorPos.y / _charSize.cy;

	// Если нам указали на несуществ. строку - сдвигаемся на первую существующую
	if ( _caretPos.y >= _text.size() ) 
		_caretPos.y = _text.size() - 1;

	// Если она не доступна для ввода - двигаемся еще выше
	while ( ! IsAvailableForInput( _caretPos.y, _text ) ) {
		assert( _caretPos.y > 0 );
		--_caretPos.y;
	}
	
	// Если мы все равно попали на не существующую строку - у нас ошибка в логике работы
	assert( _caretPos.y >= 0 && _caretPos.y < _text.size() );

	// Если нам указали на недействительную позицию в строке - сдвигаемся на конец y-вой строки
	NormalizeX( _caretPos, _text );
}

bool IsAvailableForInput( const UINT _rowIndex, const std::vector< StringInfo > & _text ) {
	// Мы не можем писать в не существующую строку
	if ( _rowIndex < 0 || _rowIndex >= _text.size() )
		return false;

	// Мы всегда можем писать в первую строку
	if ( _rowIndex == 0 )
		return true;

	LPCTSTR rowData = _text.at( _rowIndex ).first;

	// Последняя строка доступна для ввода только тогда,...
	if ( _rowIndex == ( _text.size() - 1 ) ) {	
		assert( _text.at( _rowIndex ).second == 0 );

		// ...когда она идет сразу после символа перевода строки
		return *rowData == '\n';
	}
	
	// Во всех остальных случаях строка доступна для ввода
	return true;
}

void NormalizeX( POINT & _caretPos, const std::vector< StringInfo > & _text ) {
	// Если текущая строка короче, чем запрошенная позиция...
	if ( _caretPos.x > _text.at( _caretPos.y ).second ) {
		// ...то каретка будет указывать на конец этой строки
		_caretPos.x = _text.at( _caretPos.y ).second;
	}

	// Если мы попали на символ конца строки - сдвигаемся влево на один символ
	if ( IsPointsToNewline( _caretPos, _text ) ) {
		--_caretPos.x;
	}
}
