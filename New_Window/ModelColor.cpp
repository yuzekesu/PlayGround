#include "Char4.h"
#include "Exception.h"
#include "Float3.h"
#include "Model.h"
#include "ModelColor.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgiformat.h>

ModelColor::ModelColor(ComPtr<ID3D11Device> p_device, ComPtr<ID3D11DeviceContext> p_context) {
	_vertices = {
		{ Float3{0.0f, 0.5f, 0.0f}, Char4{255u, 0u, 0u, 255u} },
		{ Float3{0.5f, -0.5f, 0.0f}, Char4{0u, 255u, 0u, 255u} },
		{ Float3{-0.5f, -0.5f, 0.0f}, Char4{0u, 0u, 255u, 255u} }
	};
	_indices = { 0u, 1u, 2u };
	Initialize_VertexBuffer(p_device);
	Initialize_IndexBuffer(p_device);
	_p_context = p_context;
}

void ModelColor::Standing_By() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_p_context->IASetVertexBuffers(0, 1, _p_vertex_buffer.GetAddressOf(), &stride, &offset);
	_p_context->IASetIndexBuffer(_p_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_p_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelColor::Initialize_VertexBuffer(ComPtr<ID3D11Device> p_device) {
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	UINT size = static_cast<UINT>(sizeof(Vertex) * _vertices.size());
	buffer_desc.ByteWidth = size;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA init_data{};
	init_data.pSysMem = _vertices.data();
	HRESULT hr = p_device->CreateBuffer(&buffer_desc, &init_data, &_p_vertex_buffer);
	CHECK(hr);
}

void ModelColor::Initialize_IndexBuffer(ComPtr<ID3D11Device> p_device) {
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	UINT size = static_cast<UINT>(sizeof(UINT) * _indices.size());
	buffer_desc.ByteWidth = size;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA init_data{};
	init_data.pSysMem = _indices.data();
	HRESULT hr = p_device->CreateBuffer(&buffer_desc, &init_data, &_p_index_buffer);
	CHECK(hr);
}
