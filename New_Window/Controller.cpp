#include "Controller.h"
#include "Direction.h"
#include "Float3.h"
#include "Float4x4.h"
#include "Float4x4_DESC.h"
#include "Model.h"
#include "ModelColor.h"
#include "NormalizedColor.h"
#include "Shader.h"
#include "ShaderColor.h"
#include "Unit.h"
#include <Windows.h>
#include <cmath>
#include <d3d11.h>
#include <memory>
#include <numbers>
#include <wrl/client.h>

/// <summary>
/// Constructor.
/// </summary>
Controller::Controller() {
	Test_Function();
	Loop();
}
Float4x4 Controller::Get_PVW(const Unit& camera, const Unit& object, const MATRIX_PERSPECTIVE_PROJECTION_DESC& pd) {
	MATRIX_VIEW_DESC vd;
	vd.position = camera.Get_Position();
	vd.right = camera.Get_Direction().Get_Right();
	vd.up = camera.Get_Direction().Get_Up();
	vd.forward = camera.Get_Direction().Get_Forward();
	MATRIX_WORLD_DESC wd;
	wd.position = object.Get_Position();
	wd.right = object.Get_Direction().Get_Right();
	wd.up = object.Get_Direction().Get_Up();
	wd.forward = object.Get_Direction().Get_Forward();
	return Float4x4::Matrix_PVW(pd, vd, wd);
}
/// <summary>
/// Graphic things.
/// </summary>
void Controller::Do_Graphic() {
	NormalizedColor color = NormalizedColor::Convert(245u, 90u, 130u, 255u);
	static float t = 0.f;
	t += _stopwatch.Get_Duration_In_Second();
	for (int i = 0; i < 4 - 1; i++) {
		color[i] = color[i] * sinf(t) / 2 + 0.5f;
	}
	_graphic.Begin_Scene(color);
	//--------------------------------------------------------------------
	//Any other rendering here.
	_graphic.Render_Units(_units);
	//_units[0]->Modify_Direction().Increase_Pitch(0.01f);
	//_units[0]->Modify_Direction().Increase_Yaw(0.01f);
	//_units[0]->Modify_Direction().Increase_Roll(0.01f);
	_units[1]->Modify_Direction().Increase_Pitch(0.01f);
	//_units[1]->Modify_Direction().Increase_Yaw(0.01f);
	//_units[1]->Modify_Direction().Increase_Roll(0.01f);
	Test_Function_Repeat();
	//--------------------------------------------------------------------
	_graphic.End_Scene();
}
/// <summary>
/// The logical loop.
/// </summary>
void Controller::Loop() {
	MSG msg{};
	_stopwatch.Set_Interval(_graphic.Get_Refresh_Rate());
	while (_app.Running) {
		_stopwatch.Standing_By();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;

			// do if has message.
		}
		Do_Graphic();
	}
	_app.Running = 0;
}

void Controller::Test_Function() {
	using namespace std;
	using namespace Microsoft::WRL;
	ComPtr<ID3D11Device> device = _graphic.Get_Device();
	ComPtr<ID3D11DeviceContext> context = _graphic.Get_Context();
	Direction direction_1; // forward: (0,0,1)
	// direction_1.Increase_Pitch(3.14f); //now backward: (0,0,-1)
	Float3 pos_1{ 0.0f, 0.0f, 5.0f };
	Float3 offset_camera_1{ 0.0f, 0.0f, 0.0f };
	shared_ptr<Model> model_1 = make_shared<ModelColor>(device, context);
	Float4x4 matrix = Float4x4::Matrix_Identical();
	shared_ptr<Shader> shader_1 = make_shared<ShaderColor>(device, context, &matrix, sizeof(matrix));
	_units.push_back(std::make_shared<Unit>(direction_1, pos_1, offset_camera_1, model_1, shader_1));

	Direction direction_2; // forward: (0,0,1)
	Float3 pos_2{ 0.0f, 0.0f, 0.0f };
	Float3 offset_camera_2{ 0.0f, 0.0f, 0.0f };
	_units.push_back(std::make_shared<Unit>(direction_2, pos_2, offset_camera_2, model_1, shader_1));

	//// world matrix test
	//Float3 xDir = direction_1.Get_Right();
	//Float3 yDir = direction_1.Get_Up();
	//Float3 zDir = direction_1.Get_Forward();
	//Float4x4 world_matrix = Float4x4::Matrix_World(xDir, yDir, zDir, pos_1);
	//// view matrix test
	//Float3 xHat = direction_2.Get_Right();
	//Float3 yHat = direction_2.Get_Up();
	//Float3 zHat = direction_2.Get_Forward();
	//Float4x4 view_matrix = Float4x4::Matrix_View(pos_2, xHat, yHat, zHat);
	//// perspective matrix test
	//const LONG x = _graphic.Get_Directx().Get_Window().GET_SIZE().cx;
	//const LONG y = _graphic.Get_Directx().Get_Window().GET_SIZE().cy;
	//float ratio = static_cast<float>(y) / static_cast<float>(x);
	//Float4x4 perspective_matrix = Float4x4::Matrix_Perspective_Projection(3.f, 300.f, 3.14f / 2, ratio);

	MATRIX_PERSPECTIVE_PROJECTION_DESC pd;
	float cx = _graphic.Get_Directx().Get_Window().GET_SIZE().cx;
	float cy = _graphic.Get_Directx().Get_Window().GET_SIZE().cy;
	pd.aspect_ratio = cx / cy;
	pd.far_plane = 300.f;
	pd.near_plane = 1.f;
	pd.fov = static_cast<float>(std::numbers::pi) / 2.f;
	matrix = Get_PVW(*_units[0], *_units[1], pd);
	shader_1->Update_ConstantBuffer(&matrix, sizeof(matrix));
}

void Controller::Test_Function_Repeat() {
	MATRIX_PERSPECTIVE_PROJECTION_DESC pd;
	float cx = _graphic.Get_Directx().Get_Window().GET_SIZE().cx;
	float cy = _graphic.Get_Directx().Get_Window().GET_SIZE().cy;
	pd.aspect_ratio = cx / cy;
	pd.far_plane = 300.f;
	pd.near_plane = 1.f;
	pd.fov = static_cast<float>(std::numbers::pi) / 2;
	Float4x4 matrix = Get_PVW(*_units[1], *_units[0], pd);
	_units[0]->Get_Shader().Update_ConstantBuffer(&matrix, sizeof(Float4x4));
}


