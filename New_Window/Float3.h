#pragma once
class Float3 {
public:
	Float3() : _x(0), _y(0), _z(0) {}
	Float3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
	Float3 operator+(const Float3& other);
	Float3 operator-(const Float3& other);
	Float3 operator*(float scalar);
	Float3 operator/(float denominator);
	float operator*(const Float3& other) const;
	void Normalize();
	void operator+=(const Float3& other);
	void operator-=(const Float3& other);
	void operator*=(const float scalar);
	void operator/=(const float denominator);
public:
	float _x, _y, _z;

};

