#ifndef _BTN_CONTROLLER_HPP_
#define _BTN_CONTROLLER_HPP_

/*****************************************************************************/
enum class FigureID;

#include <windows.h>

#include <string>
#include <vector>
#include <array>

/*****************************************************************************/

// �����, ������� ���������� ��������� � ���������� ���� ������
class BtnController {
public:
	// ������������� ���� �������������� ������
	enum ButtonID {
		BTN_UNKNOWN = -1,
		BTN_LINE,
		BTN_ELLIPSE,
		BTN_RECTANGLE,
		N_OF_BUTTONS,
	};

	// �����������, ��������� ��������� � ����������� � �������
	explicit BtnController();

	void CreateWindows( const HWND _parentWindow );

	// ������������ ��������� � ������� ������
	ButtonID HandleClick( const UINT _message, const WPARAM _wParam, const LPARAM _lParam );

private:
	// ��������� � ����������� � ������
	struct ButtonInfo
	{
		std::basic_string< TCHAR > m_name; // ��� ������
		LONG m_style; // ����� ������
		HWND m_hWnd; // ��������� ���� ������
	};

	// ������ ������
	std::array< ButtonInfo, (SIZE_T)ButtonID::N_OF_BUTTONS > m_buttons;

	void InitButtons();
};

#endif // _BTN_CONTROLLER_HPP_

/*****************************************************************************/
