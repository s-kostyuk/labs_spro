#include <windows.h>         // ����������� ���������� � ��������� API
#include "resource1.h"

// ���������� ����������:
HINSTANCE hInst; 	// ��������� ����������
LPCTSTR szWindowClass = "Kostyuk";
LPCTSTR szTitle = "Simple app";

const int xWindowSize = 400;
const int yWindowSize = 250;

// ��������������� �������� �������

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// �������� ��������� 
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	// ����������� ������ ���� 
	MyRegisterClass(hInstance);

	// �������� ���� ����������
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  ������������ ����� ���� 

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_DBLCLKS;	// ����� ����
	wcex.lpfnWndProc = (WNDPROC)WndProc; // ������� ���������
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;		// ��������� ����������
	wcex.hIcon = LoadIcon(NULL, IDI_INFORMATION);		// ����������� ������
	wcex.hCursor = LoadCursor(NULL, IDC_WAIT);    // ����������� �������
	wcex.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );   // ��������� ����
	wcex.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );		// ����������� ����
	wcex.lpszClassName = szWindowClass;	// ��� ������
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex); // ����������� ������ ����
}

// FUNCTION: InitInstance(HANDLE, int)
// ������� ���� ���������� � ��������� ��������� ���������� � ���������� hInst

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	const int xScreen = GetSystemMetrics(SM_CXSCREEN);
	const int yScreen = GetSystemMetrics(SM_CYSCREEN);

	const int xWindowPosition = (xScreen - xWindowSize) / 2;
	const int yWindowPosition = (yScreen - yWindowSize) / 2;

	HWND hWnd;

	hInst = hInstance; // ��������� ��������� ���������� � ���������� hInst

	hWnd = CreateWindow(szWindowClass, // ��� ������ ����
		szTitle,   // ��� ����������
		WS_VSCROLL | WS_MINIMIZEBOX | WS_MINIMIZE | WS_TILED | WS_SYSMENU | WS_SIZEBOX, // ����� ����
		xWindowPosition,	// ��������� �� �
		yWindowPosition, 	// ��������� �� Y
		xWindowSize,    // ������ �� �
		yWindowSize,    // ������ �� Y
		NULL,	// ��������� ������������� ����
		NULL,       // ��������� ���� ����
		hInstance,  // ��������� ����������
		NULL);     // ��������� ��������.

	if (!hWnd) // ���� ���� �� ���������, ������� ���������� FALSE
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_MINIMIZE);		// �������� ����
	UpdateWindow(hWnd);			// �������� ����
	return TRUE;				//�������� ���������� �������
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  ������� ���������. ��������� � ������������ ��� ���������, ���������� � ����������


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	switch (message)
	{
	case WM_CREATE: // ��������� �������� ��� �������� ����
		break;

	case WM_PAINT:  // ������������ ����
		hdc = BeginPaint(hWnd, &ps);	// ������ ����������� �����
		GetClientRect(hWnd, &rt); // ������� ���� ��� ���������
		DrawText(hdc, "[Make a double click here]", -1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);	// ��������� ����������� �����
		break;

	case WM_DESTROY: // ���������� ������
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDBLCLK:
		MessageBox(hWnd, "Congratulations! You made double a click", "Greeting", MB_OK);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_ABOUT_ABOUT:
			MessageBox(hWnd, "Created by Sergey Kostyuk, gr. CE-14-3", "About", MB_OK);
			break;

		case ID_CHANGE_COURSOR1:
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)) );
			break;

		case ID_CHANGE_COURSOR2:
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR2)));
			break;

		case ID_CHANGE_CURSOR3:
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR3)));
			break;
		}
	default:
		// ��������� ���������, ������� �� ���������� �������������
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}