#pragma once
#include "Float3.h"

struct MATRIX_PERSPECTIVE_PROJECTION_DESC {
	float near_plane;
	float far_plane;
	float fov;
	float aspect_ratio;
};
struct MATRIX_VIEW_DESC {
	Float3 position;
	Float3 right;
	Float3 up;
	Float3 forward;
};
struct MATRIX_WORLD_DESC {
	Float3 position;
	Float3 right;
	Float3 up;
	Float3 forward;
};