#include "Direction.h"
#include "Float3.h"
#include <cmath>
#include <numbers>
/// <summary>
/// Increasing will turn "RIGHT Horizontally".
/// </summary>
/// <param name="radian"></param>
void Direction::Increase_Yaw(float radian) {
	Rotate(_up, _forward, radian);
}
/// <summary>
/// Increasing will turn "Down Vertically"
/// </summary>
/// <param name="radian"></param>
void Direction::Increase_Pitch(float radian) {
	Rotate(_forward, _right, radian);
}
/// <summary>
/// Increasing will turn "Counter-Clockwise"
/// </summary>
/// <param name="radian"></param>
void Direction::Increase_Roll(float radian) {
	Rotate(_right, _up, radian);
}
/// <summary>
/// Internal helper function that rotate the Direction as itself as the current coordinate system.
/// </summary>
/// <param name="x_rotate"> Assume it is the x_hat and the vector to be rotated.</param>
/// <param name="y_rotate"> Assume it is the y_hat and the vector to be rotated. </param>
/// <param name="radian">Theta.</param>
void Direction::Rotate(Float3& x_rotate, Float3& y_rotate, float radian) {
	float pi = static_cast<float>(std::numbers::pi);
	float cx = cosf(radian);
	float sx = sin(radian);
	float cy = cosf(radian + pi / 2);
	float sy = sin(radian + pi / 2);
	Float3 x_new = x_rotate * cx + y_rotate * sx;
	Float3 y_new = x_rotate * cy + y_rotate * sy;
	x_rotate = x_new;
	y_rotate = y_new;
}
