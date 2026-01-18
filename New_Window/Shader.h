#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <d3dcommon.h>
#include <wrl/client.h>

class Shader {
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	virtual void Standing_By() const = 0;
	virtual void Update_ConstantBuffer(const void* cbuffer_ptr, size_t cbuffer_size) const = 0;
protected:
	Shader() = default;
	virtual void Initialize_ConstantBuffer(ComPtr<ID3D11Device> p_device, const void* cbuffer_ptr, size_t cbuffer_size) = 0;
	virtual void Initialize_InputLayout(ComPtr<ID3D11Device> p_device, ComPtr<ID3DBlob> p_vertex_shader_bytecode) = 0;
	virtual void Initialize_PixelShader(ComPtr<ID3D11Device> p_device) = 0;
	virtual void Initialize_VertexShader(ComPtr<ID3D11Device> p_device) = 0;
protected:
	ComPtr<ID3D11Buffer> _p_constant_buffer;
	ComPtr<ID3D11DeviceContext> _p_context;
	ComPtr<ID3D11InputLayout> _p_input_layout;
	ComPtr<ID3D11PixelShader> _p_pixel_shader;
	ComPtr<ID3D11VertexShader> _p_vertex_shader;
	size_t _buffer_size = 0u;
};

