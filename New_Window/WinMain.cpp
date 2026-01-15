#include <Windows.h>
#include "Controller.h"
#include "Exception.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE UNUSED_hInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		Controller controller{};
	}
	catch (Exception e) {
		MessageBoxA(NULL, e.what(), "Exception", 0);
	}
	return 0;
}