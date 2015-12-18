#ifndef _FIGURES_HPP_
#define _FIGURES_HPP_

/*****************************************************************************/

#include <windows.h>
#include <vector>

#include "support.hpp"

/*****************************************************************************/

// Типы поддерживаемых фигур
enum class FigureType {
	LINE,
	ELLIPSE,
	RECTANGLE
};

// Структура с информацией о фигуре
struct FigureInfo
{
	FigureType m_type;
	RECT m_dim;
};

/*****************************************************************************/

void DrawSavedObjects( HDC _hdc, const std::vector< FigureInfo > & _saved );

void DrawObject( HDC _hdc, const FigureType _type, const RECT & _objectCoordinates );

/*****************************************************************************/

#endif // !_FIGURES_HPP_
