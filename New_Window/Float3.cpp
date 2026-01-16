#include "float3.h"
#include <cmath>

float Float3::operator*(const Float3& other) const {
	return _x * other._x + _y * other._y + _z * other._z;
}

void Float3::normalize() {
	float length = sqrt(_x * _x + _y * _y + _z * _z);
	if (length > 0) {
		_x /= length;
		_y /= length;
		_z /= length;
	}
}

void Float3::operator+=(const Float3& other) {
	_x += other._x;
	_y += other._y;
	_z += other._z;
}

void Float3::operator-=(const Float3& other) {
	_x -= other._x;
	_y -= other._y;
	_z -= other._z;
}

void Float3::operator*=(const float scalar) {
	_x *= scalar;
	_y *= scalar;
	_z *= scalar;
}

void Float3::operator/=(const float scalar) {
	_x /= scalar;
	_y /= scalar;
	_z /= scalar;
}
