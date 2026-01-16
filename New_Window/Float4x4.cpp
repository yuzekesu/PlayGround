#include "Float3.h"
#include "Float4x4.h"
#include <array>
#include <cmath>

Float4x4::Float4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
	_matrix[0][0] = m00;
	_matrix[1][0] = m01;
	_matrix[2][0] = m02;
	_matrix[3][0] = m03;

	_matrix[0][1] = m10;
	_matrix[1][1] = m11;
	_matrix[2][1] = m12;
	_matrix[3][1] = m13;

	_matrix[0][2] = m20;
	_matrix[1][2] = m21;
	_matrix[2][2] = m22;
	_matrix[3][2] = m23;

	_matrix[0][3] = m30;
	_matrix[1][3] = m31;
	_matrix[2][3] = m32;
	_matrix[3][3] = m33;
}

Float4x4 Float4x4::operator()(const Float4x4& other) const {
	Float4x4 matrix;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++)
			for (int i = 0; i < 4; i++) {
				matrix[r][c] += _matrix[r][i] * other[i][c];
			}
	}
	return matrix;
}

std::array<float, 4u>& Float4x4::operator[](size_t index) {
	return _matrix[index];
}

const std::array<float, 4u>& Float4x4::operator[](size_t index) const {
	return _matrix[index];
}

Float4x4 Float4x4::Matrix_Identical() {
	Float4x4 matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	return matrix;
}

Float4x4 Float4x4::Matrix_Perspective_Projection(float near, float far, float fov, float yDx_relation) {
	float s = 1.f / tanf(fov / 2);
	Float4x4 matrix(
		yDx_relation * s,/*	*/ 0.f,/*	*/ 0.f,/*					*/ 0.f,
		0.f,/*				*/  s,/*	*/ 0.f,/*					*/ 0.f,
		0.f,/*				*/ 0.f,/*	*/ far / (far - near),/*	*/ -far * near / (far - near),
		0.f,/*				*/ 0.f,/*	*/ 1.f,/*					*/ 0.f
	);
	return matrix;
}

Float4x4 Float4x4::Matrix_View(const Float3& pos, const Float3& vec_right, const Float3& vec_up, const Float3& vec_forward) {
	Float4x4 matrix(
		vec_right._x,/*		*/ vec_right._y,/*	*/ vec_right._z,/*	*/ -(pos * vec_right),
		vec_up._x,/*		*/ vec_up._y,/*		*/ vec_up._z,/*		*/ -(pos * vec_up),
		vec_forward._x,/*	*/ vec_forward._y,/**/ vec_forward._z,/**/ -(pos * vec_forward),
		0,/*				*/ 0,/*				*/ 0,/*				*/ 1
	);
	return matrix;
}
