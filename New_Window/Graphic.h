#pragma once
#include "Directx.h"
#include "NormalizedColor.h"

class Graphic
{
public:
	float Get_Refresh_Rate();
	void Begin_Scene(NormalizedColor normalized_color);
	void End_Scene();
private:
private:
	Directx _direct_x;
};

