#include "win.h"

window::window(const char * const _title, int _width, int _height) noexcept
	: hWnd(nullptr)
{
	RegisterWndClass(WndProc, TEXT(_title), COLOR_WINDOW);
	hWnd = CreateWindow(TEXT(_title),
		TEXT(_title),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		0,
		_width,
		_height,
		(HWND)NULL,
		(HMENU)NULL,
		(HINSTANCE)NULL,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Cant create window"), TEXT("Error"), MB_OK);
	}
}

void window::show() noexcept
{
	ShowWindow(hWnd, SW_SHOW);
}

HWND window::handle() noexcept
{
	return hWnd;
}

LRESULT CALLBACK window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

void window::RegisterWndClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)brBackground;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Cannot register class"), TEXT("Error"), MB_OK);
	}
}