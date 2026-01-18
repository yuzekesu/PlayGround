#pragma once
#include "App.h"
#include "Float4x4.h"
#include "Float4x4_DESC.h"
#include "Graphic.h"
#include "Stopwatch.h"
#include "Unit.h"
#include <memory>
#include <vector>

class Controller {
public:
	Controller();
	Float4x4 Get_PVW(const Unit& camera, const Unit& object, const MATRIX_PERSPECTIVE_PROJECTION_DESC& pd);
	void Do_Graphic();
	void Loop();
	void Test_Function();
	void Test_Function_Repeat();
public:
	App _app;
	Graphic _graphic;
	Stopwatch _stopwatch;
	std::vector<std::shared_ptr<Unit>> _units;
};
