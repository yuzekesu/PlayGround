#pragma once
#pragma comment(lib, "d3d11.lib")
#include "Directx.h"
#include "NormalizedColor.h"
#include "Unit.h"
#include <d3d11.h>
#include <memory>
#include <vector>
#include <wrl/client.h>

class Graphic {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ComPtr<ID3D11Device> Get_Device() { return _direct_x.Get_Device(); }
	ComPtr<ID3D11DeviceContext> Get_Context() { return _direct_x.Get_Context(); }
	const Directx& Get_Directx() const { return _direct_x; }
	float Get_Refresh_Rate();
	void Begin_Scene(NormalizedColor normalized_color);
	void End_Scene();
	void Render_Units(std::vector<std::shared_ptr<Unit>> units);
private:
	void Render_Unit(Unit& unit);
private:
	Directx _direct_x;
};

