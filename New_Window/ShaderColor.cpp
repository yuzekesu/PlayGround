#include "Exception.h"
#include "ShaderColor.h"
#include <Windows.h>
#include <cstring>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <dxgiformat.h>

ShaderColor::ShaderColor(ComPtr<ID3D11Device> p_device, ComPtr<ID3D11DeviceContext> p_context, const void* cbuffer_ptr, size_t cbuffer_size) {
	Initialize_VertexShader(p_device);
	Initialize_PixelShader(p_device);
	Initialize_ConstantBuffer(p_device, cbuffer_ptr, cbuffer_size);
	_p_context = p_context;
	_buffer_size = cbuffer_size;
}

void ShaderColor::Standing_By() const {
	_p_context->IASetInputLayout(_p_input_layout.Get());
	_p_context->VSSetShader(_p_vertex_shader.Get(), nullptr, 0);
	_p_context->PSSetShader(_p_pixel_shader.Get(), nullptr, 0);
	_p_context->VSSetConstantBuffers(0, 1, _p_constant_buffer.GetAddressOf());
}

void ShaderColor::Update_ConstantBuffer(const void* cbuffer_ptr, size_t cbuffer_size) const {
	if (cbuffer_size != _buffer_size) {
		throw Exception("Constant Buffer Size Mismatch in ShaderColor::Update_ConstantBuffer");
	}
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	HRESULT hr = _p_context->Map(_p_constant_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	CHECK(hr);
	UINT size = static_cast<UINT>(cbuffer_size);
	memcpy(mapped_resource.pData, cbuffer_ptr, size);
	_p_context->Unmap(_p_constant_buffer.Get(), 0);
}

void ShaderColor::Initialize_ConstantBuffer(ComPtr<ID3D11Device> p_device, const void* cbuffer_ptr, size_t cbuffer_size) {
	UINT size = static_cast<UINT>(cbuffer_size);
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.ByteWidth = size;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_SUBRESOURCE_DATA init_data{};
	init_data.pSysMem = cbuffer_ptr;
	HRESULT hr = p_device->CreateBuffer(&buffer_desc, &init_data, &_p_constant_buffer);
	CHECK(hr);
}

void ShaderColor::Initialize_InputLayout(ComPtr<ID3D11Device> p_device, ComPtr<ID3DBlob> p_vertex_shader_bytecode) {
	// float3 Pos : POSITION;
	// float4 Color : COLOR0;
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC position;
	D3D11_INPUT_ELEMENT_DESC color;
	position.AlignedByteOffset = 0;
	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	position.InputSlot = 0;
	position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	position.InstanceDataStepRate = 0;
	position.SemanticIndex = 0;
	position.SemanticName = "POSITION";
	color.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	color.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	color.InputSlot = 0;
	color.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	color.InstanceDataStepRate = 0;
	color.SemanticIndex = 0;
	color.SemanticName = "COLOR";
	D3D11_INPUT_ELEMENT_DESC layout_desc[] = { position, color };
	UINT num_elements = sizeof(layout_desc) / sizeof(layout_desc[0]);
	void* p_bytecode = p_vertex_shader_bytecode->GetBufferPointer();
	size_t size_bytecode = p_vertex_shader_bytecode->GetBufferSize();
	hr = p_device->CreateInputLayout(layout_desc, num_elements, p_bytecode, size_bytecode, &_p_input_layout);
	CHECK(hr);
}

void ShaderColor::Initialize_PixelShader(ComPtr<ID3D11Device> p_device) {
	using namespace Microsoft::WRL;
	HRESULT hr;
	ComPtr<ID3DBlob> p_error;
	ComPtr<ID3DBlob> p_pixel_shader_bytecode;
	hr = D3DCompileFromFile(L"Color.ps.hlsl", nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_pixel_shader_bytecode, p_error.ReleaseAndGetAddressOf());
	if (FAILED(hr)) {
		if (p_error) {
			char* p_error_msg = static_cast<char*>(p_error->GetBufferPointer());
			MessageBoxA(NULL, p_error_msg, "Pixel Shader Compilation Error", MB_OK | MB_ICONERROR);
		}
		CHECK(hr);
	}
	void* p_bytecode = p_pixel_shader_bytecode->GetBufferPointer();
	size_t size_bytecode = p_pixel_shader_bytecode->GetBufferSize();
	hr = p_device->CreatePixelShader(p_bytecode, size_bytecode, nullptr, &_p_pixel_shader);
	CHECK(hr);
}

void ShaderColor::Initialize_VertexShader(ComPtr<ID3D11Device> p_device) {
	using namespace Microsoft::WRL;
	HRESULT hr;
	ComPtr<ID3DBlob> p_error;
	ComPtr<ID3DBlob> p_vertex_shader_bytecode;
	hr = D3DCompileFromFile(L"Color.vs.hlsl", nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &p_vertex_shader_bytecode, p_error.ReleaseAndGetAddressOf());
	if (FAILED(hr)) {
		if (p_error) {
			char* p_error_msg = static_cast<char*>(p_error->GetBufferPointer());
			MessageBoxA(NULL, p_error_msg, "Vertex Shader Compilation Error", MB_OK | MB_ICONERROR);
		}
		CHECK(hr);
	}
	void* p_bytecode = p_vertex_shader_bytecode->GetBufferPointer();
	size_t size_bytecode = p_vertex_shader_bytecode->GetBufferSize();
	hr = p_device->CreateVertexShader(p_bytecode, size_bytecode, nullptr, &_p_vertex_shader);
	CHECK(hr);
	Initialize_InputLayout(p_device, p_vertex_shader_bytecode);
}

