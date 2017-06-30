#ifndef _WIN_H
#define _WIN_H

#include <windows.h>

class window
{
public:
	window() = default;
	window(const char * const _title, int _width, int _height) noexcept;

	void show() noexcept;
	HWND handle() noexcept;

private:
	HWND hWnd;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void RegisterWndClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground);
};

#endif