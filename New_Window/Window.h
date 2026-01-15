#pragma once
#include <Windows.h>
class Window
{
private:
	HWND _hwnd;
	SIZE _size;
	RECT _rect;
public:
	const HWND& Get_HWND();
	const SIZE& GET_SIZE();
	const RECT& Get_RECT();
public:
	Window();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	void Fill_WNDCLASSEX(WNDCLASSEX& wndclass, LPCTSTR classname);
	void Fill_SIZE_RECT();
};

