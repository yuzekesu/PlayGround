#pragma once
#include "Float3.h"
#include "Float4x4_DESC.h"
#include <array>

class Float4x4 {
public:
	Float4x4(float r0c0, float r0c1, float r0c2, float r0c3, float r1c0, float r1c1, float r1c2, float r1c3, float r2c0, float r2c1, float r2c2, float r2c3, float r3c0, float r3c1, float r3c2, float r3c3);
	Float4x4 operator()(const Float4x4& other) const;
	std::array<float, 4u>& operator[](size_t index);
	const std::array<float, 4u>& operator[](size_t index) const;
	static Float4x4 Matrix_Identical();
	static Float4x4 Matrix_Perspective_Projection(const MATRIX_PERSPECTIVE_PROJECTION_DESC& desc);
	static Float4x4 Matrix_Translation(const Float3& translation);
	static Float4x4 Matrix_View(const MATRIX_VIEW_DESC& desc);
	static Float4x4 Matrix_World(const MATRIX_WORLD_DESC& desc);
	static Float4x4 Matrix_PVW(const MATRIX_PERSPECTIVE_PROJECTION_DESC& pd, const MATRIX_VIEW_DESC& vd, const MATRIX_WORLD_DESC& wd);
private:
	Float4x4() : _matrix{} {};
private:
	std::array<std::array<float, 4u>, 4u> _matrix;
};
static_assert(sizeof(Float4x4) == 64, "Float4x4 must be 64 bytes.");

