#pragma once
#include "App.h"
#include "graphic.h"
#include "Stopwatch.h"
class Controller {
public:
	Controller();
public:
	void Do_Graphic();
	void Loop();
public:
	App _app;
	Graphic _graphic;
	Stopwatch _stopwatch;
};
