#include "Controller.h"
#include "NormalizedColor.h"
#include <cmath>
#include <Windows.h>

/// <summary>
/// Constructor.
/// </summary>
Controller::Controller() {
	Loop();
}
/// <summary>
/// Graphic things.
/// </summary>
void Controller::Do_Graphic()
{
	NormalizedColor color = NormalizedColor::Convert(245u, 90u, 130u, 255u);
	static float t = 0.f;
	t += _stopwatch.Get_Duration_In_Second();
	for (int i = 0; i < 4 - 1; i++) {
		color[i] = color[i] * sinf(t) / 2 + 0.5f;
	}
	_graphic.Begin_Scene(color);
	//--------------------------------------------------------------------
	//Any other rendering here.

	//--------------------------------------------------------------------
	_graphic.End_Scene();
}
/// <summary>
/// The logical loop.
/// </summary>
void Controller::Loop() {
	MSG msg{};
	_stopwatch.Set_Interval(_graphic.Get_Refresh_Rate());
	while (_app.Running) {
		_stopwatch.Standing_By();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;

			// do if has message.
		}
		Do_Graphic();
	}
	_app.Running = 0;
}


