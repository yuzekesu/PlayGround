#pragma once
#pragma comment(lib, "d3d11.lib")
#include "Shader.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <wrl/client.h>

class ShaderColor final : public Shader {
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	ShaderColor() = delete;
	ShaderColor(ComPtr<ID3D11Device> p_device, ComPtr<ID3D11DeviceContext> p_context, const void* cbuffer_ptr, size_t cbuffer_size);
	void Standing_By() const override;
	void Update_ConstantBuffer(const void* cbuffer_ptr, size_t cbuffer_size) const override;
protected:
	void Initialize_ConstantBuffer(ComPtr<ID3D11Device> p_device, const void* cbuffer_ptr, size_t cbuffer_size) override;
	void Initialize_InputLayout(ComPtr<ID3D11Device> p_device, ComPtr<ID3DBlob> p_vertex_shader_bytecode) override;
	void Initialize_PixelShader(ComPtr<ID3D11Device> p_device) override;
	void Initialize_VertexShader(ComPtr<ID3D11Device> p_device) override;
};

