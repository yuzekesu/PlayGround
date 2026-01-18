#pragma once
#include "Float3.h"

class Direction {
public:
	const Float3& Get_Right() const { return _right; }
	const Float3& Get_Up() const { return _up; }
	const Float3& Get_Forward() const { return _forward; }
	void Increase_Yaw(float radian);
	void Increase_Pitch(float radian);
	void Increase_Roll(float radian);
private:
	void Rotate(Float3& x_rotate, Float3& y_rotate, float radian);
private:
	Float3 _right{ 1,0,0 };
	Float3 _up{ 0,1,0 };
	Float3 _forward{ 0,0,1 };
};

