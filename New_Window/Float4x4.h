#pragma once
#include "Float3.h"
#include <array>

class Float4x4 {
public:
	Float4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
	Float4x4 operator()(const Float4x4& other) const;
	std::array<float, 4u>& operator[](size_t index);
	const std::array<float, 4u>& operator[](size_t index) const;
	static Float4x4 Matrix_Identical();
	static Float4x4 Matrix_Perspective_Projection(float near, float far, float fov, float yDx_relation);
	static Float4x4 Matrix_View(const Float3& pos, const Float3& vec_right, const Float3& vec_up, const Float3& vec_forward);
private:
	Float4x4() : _matrix{} {};
private:
	std::array<std::array<float, 4u>, 4u> _matrix;
};
static_assert(sizeof(Float4x4) == 64, "Float4x4 must be 64 bytes.");

