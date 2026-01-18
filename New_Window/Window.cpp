#include "Window.h"
#include <Windows.h>
#include <libloaderapi.h>

const HWND& Window::Get_HWND() {
	return _hwnd;
}

const SIZE& Window::GET_SIZE() const {
	return _size;
}

const RECT& Window::Get_RECT() {
	return _rect;
}

Window::Window() {
	WNDCLASSEX wndclass{};
	LPCTSTR classname = L"my window";
	Fill_WNDCLASSEX(wndclass, classname);
	RegisterClassEx(&wndclass);
	Fill_SIZE_RECT();
	int width = _size.cx;
	int height = _size.cy;
	int x = _rect.left;
	int y = _rect.top;
	_hwnd = CreateWindowEx(WS_EX_LAYERED, classname, classname, WS_POPUP, x, y, width, height, NULL, NULL, GetModuleHandleA(NULL), nullptr);
	SetLayeredWindowAttributes(_hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
	ShowWindow(_hwnd, SW_SHOW);
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

void Window::Fill_WNDCLASSEX(WNDCLASSEX& wndclass, LPCTSTR classname) {
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = 0;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandleA(NULL);
	wndclass.hIcon = NULL;
	wndclass.hCursor = GetCursor();
	wndclass.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = classname;
	wndclass.hIconSm = NULL;
}

void Window::Fill_SIZE_RECT() {
	_size.cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	_size.cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	_rect.left = _size.cx / 2;
	_rect.top = _size.cy / 2;
	_rect.right = _rect.left + _size.cx;
	_rect.bottom = _rect.top + _size.cy;
}
