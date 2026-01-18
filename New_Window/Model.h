#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl/client.h>
class Model {
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	virtual void Standing_By() = 0;
	virtual size_t Get_Indices_Count() const = 0;
protected:
	Model() = default;
	virtual void Initialize_VertexBuffer(ComPtr<ID3D11Device> p_device) = 0;
	virtual void Initialize_IndexBuffer(ComPtr<ID3D11Device> p_device) = 0;
protected:
	ComPtr<ID3D11Buffer> _p_vertex_buffer;
	ComPtr<ID3D11Buffer> _p_index_buffer;
	ComPtr<ID3D11DeviceContext> _p_context;
};

