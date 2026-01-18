#pragma once
#pragma comment(lib, "d3d11.lib")
#include "Char4.h"
#include "Float3.h"
#include "Model.h"
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

class ModelColor final : public Model {
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	struct Vertex {
		Float3 Position;
		Char4 Color;
	};
	static_assert(sizeof(Vertex) == 16, "Must be 16 bytes per Vertex.");
public:
	ModelColor() = delete;
	ModelColor(ComPtr<ID3D11Device> p_device, ComPtr<ID3D11DeviceContext> p_context);
	void Standing_By() override;
	size_t Get_Indices_Count() const override { return _indices.size(); }
protected:
	void Initialize_VertexBuffer(ComPtr<ID3D11Device> p_device) override;
	void Initialize_IndexBuffer(ComPtr<ID3D11Device> p_device) override;
private:
	std::vector<Vertex> _vertices;
	std::vector<UINT> _indices;
};

