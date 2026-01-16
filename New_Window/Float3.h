#pragma once
class Float3 {
public:
	Float3() : _x(0), _y(0), _z(0) {}
	Float3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
	float operator*(const Float3& other) const;
	void normalize();
	void operator+=(const Float3& other);
	void operator-=(const Float3& other);
	void operator*=(const float scalar);
	void operator/=(const float scalar);
public:
	float _x, _y, _z;

};

