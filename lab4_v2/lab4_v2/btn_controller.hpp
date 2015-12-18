#ifndef _BTN_CONTROLLER_HPP_
#define _BTN_CONTROLLER_HPP_

/*****************************************************************************/
enum class FigureID;

#include <windows.h>

#include <string>
#include <vector>
#include <array>

/*****************************************************************************/

// Класс, который занимается хранением и обработкой всех кнопок
class BtnController {
public:
	// Идентификатоы всех поддерживаемых кнопок
	enum ButtonID {
		BTN_UNKNOWN = -1,
		BTN_LINE,
		BTN_ELLIPSE,
		BTN_RECTANGLE,
		N_OF_BUTTONS,
	};

	// Конструктор, заполняет контейнер с информацией о кнопках
	explicit BtnController();

	void CreateWindows( const HWND _parentWindow );

	// Обрабатывает сообщения о нажатии кнопки
	ButtonID HandleClick( const UINT _message, const WPARAM _wParam, const LPARAM _lParam );

private:
	// Структура с информацией о кнопке
	struct ButtonInfo
	{
		std::basic_string< TCHAR > m_name; // имя кнопки
		LONG m_style; // стиль кнопки
		HWND m_hWnd; // описатель окна кнопки
	};

	// Массив кнопок
	std::array< ButtonInfo, (SIZE_T)ButtonID::N_OF_BUTTONS > m_buttons;

	void InitButtons();
};

#endif // _BTN_CONTROLLER_HPP_

/*****************************************************************************/
