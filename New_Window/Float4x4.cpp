#include "Float3.h"
#include "Float4x4.h"
#include "Float4x4_DESC.h"
#include <array>
#include <cmath>

Float4x4::Float4x4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3) {
	_matrix[0][0] = r0c0;
	_matrix[1][0] = r0c1;
	_matrix[2][0] = r0c2;
	_matrix[3][0] = r0c3;

	_matrix[0][1] = r1c0;
	_matrix[1][1] = r1c1;
	_matrix[2][1] = r1c2;
	_matrix[3][1] = r1c3;

	_matrix[0][2] = r2c0;
	_matrix[1][2] = r2c1;
	_matrix[2][2] = r2c2;
	_matrix[3][2] = r2c3;

	_matrix[0][3] = r3c0;
	_matrix[1][3] = r3c1;
	_matrix[2][3] = r3c2;
	_matrix[3][3] = r3c3;
}

Float4x4 Float4x4::operator()(const Float4x4& other) const {
	Float4x4 matrix;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++)
			for (int i = 0; i < 4; i++) {
				matrix[r][c] += _matrix[i][c] * other[r][i];
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

Float4x4 Float4x4::Matrix_Perspective_Projection(const MATRIX_PERSPECTIVE_PROJECTION_DESC& desc) {
	const float& near_p = desc.near_plane;
	const float& far_p = desc.far_plane;
	const float& fov = desc.fov;
	const float& aspect_ratio = desc.aspect_ratio;
	const float s = 1.f / std::tanf(fov / 2);
	Float4x4 matrix(
		s / aspect_ratio,/*	*/ 0.f,/*	*/ 0.f,/*						*/ 0.f,
		0.f,/*				*/  s /** aspect_ratio*/,/*	*/ 0.f,/*						*/ 0.f,
		0.f,/*				*/ 0.f,/*	*/ far_p / (far_p - near_p),/*	*/ -far_p * near_p / (far_p - near_p),
		0.f,/*				*/ 0.f,/*	*/ 1.f,/*						*/ 0.f
	);
	return matrix;
}

Float4x4 Float4x4::Matrix_Translation(const Float3& translation) {
	Float4x4 matrix(
		1.f, 0.f, 0.f, translation._x,
		0.f, 1.f, 0.f, translation._y,
		0.f, 0.f, 1.f, translation._z,
		0.f, 0.f, 0.f, 1.f
	);
	return matrix;
}

Float4x4 Float4x4::Matrix_View(const MATRIX_VIEW_DESC& desc) {
	const Float3& pos = desc.position;
	const Float3& vec_right = desc.right;
	const Float3& vec_up = desc.up;
	const Float3& vec_forward = desc.forward;
	Float4x4 matrix(
		vec_right._x,/*		*/ vec_right._y,/*	*/ vec_right._z,/*	*/ -(pos * vec_right),
		vec_up._x,/*		*/ vec_up._y,/*		*/ vec_up._z,/*		*/ -(pos * vec_up),
		vec_forward._x,/*	*/ vec_forward._y,/**/ vec_forward._z,/**/ -(pos * vec_forward),
		0,/*				*/ 0,/*				*/ 0,/*				*/ 1
	);
	return matrix;
}

Float4x4 Float4x4::Matrix_World(const MATRIX_WORLD_DESC& desc) {
	const Float3& translation = desc.position;
	const Float3& vec_right = desc.right;
	const Float3& vec_up = desc.up;
	const Float3& vec_forward = desc.forward;
	Float4x4 matrix(
		vec_right._x,/*		*/ vec_right._y,/*	*/ vec_right._z,/*	*/ translation._x,
		vec_up._x,/*		*/ vec_up._y,/*		*/ vec_up._z,/*		*/ translation._y,
		vec_forward._x,/*	*/ vec_forward._y,/**/ vec_forward._z,/**/ translation._z,
		0,/*				*/ 0,/*				*/ 0,/*				*/ 1
	);
	return matrix;
}

Float4x4 Float4x4::Matrix_PVW(const MATRIX_PERSPECTIVE_PROJECTION_DESC& pd, const MATRIX_VIEW_DESC& vd, const MATRIX_WORLD_DESC& wd) {
	Float4x4 perspective = Float4x4::Matrix_Perspective_Projection(pd);
	Float4x4 view = Float4x4::Matrix_View(vd);
	Float4x4 world = Float4x4::Matrix_World(wd);
	return perspective(view(world));
}

