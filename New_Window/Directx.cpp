#include "Directx.h"
#include "Exception.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <dxgiformat.h>
#include <dxgitype.h>
#include<stdexcept>
#include <wrl/client.h>
using namespace Microsoft::WRL;

/// <summary>
/// Constructor.
/// </summary>
Directx::Directx() {
	Initialize_Info();
	Initialize_SwapChain_Device_DeviceContext();
	Initialize_RenderTargetView();
	Initialize_DepthStencilView();
	_p_context->OMSetRenderTargets(1, _p_render_target_view.GetAddressOf(), _p_depthstencil_view.Get());
	Initialize_DepthStencilState();
	Initialize_Rasterizer_State();
	Initialize_ViewPort();

}
/// <summary>
/// Exposing the swap chain.
/// </summary>
/// <returns></returns>
ComPtr<IDXGISwapChain> Directx::Get_SwapChain() {
	return _p_swap_chain;
}
/// <summary>
/// 
/// </summary>
/// <returns></returns>
ComPtr<ID3D11Device> Directx::Get_Device() {
	return _p_device;
}
/// <summary>
/// Exposing the device context.
/// </summary>
/// <returns></returns>
ComPtr<ID3D11DeviceContext> Directx::Get_Context() {
	return _p_context;
}
/// <summary>
/// Exposing the depth stencil view.
/// </summary>
/// <returns></returns>
ComPtr<ID3D11DepthStencilView> Directx::Get_DepthStencilView() {
	return _p_depthstencil_view;
}
/// <summary>
/// Exposing the render target view.
/// </summary>
/// <returns></returns>
ComPtr<ID3D11RenderTargetView> Directx::Get_RenderTargetView() {
	return _p_render_target_view;
}
/// <summary>
/// Exposing the refresh rate.
/// </summary>
/// <returns></returns>
float Directx::Get_Refresh_Rate() const {
	if (_refresh_rate.Denominator == 0u) {
		throw std::invalid_argument("Divide by Zero: Refresh rate.");
	}
	float n = static_cast<float>(_refresh_rate.Numerator);
	float d = static_cast<float>(_refresh_rate.Denominator);
	float result = d / n;
	//MessageBox(NULL, std::format(L"Intervall in second: {} s", result).c_str(), NULL, NULL);
	return result;
}
/// <summary>
/// Getting the VRAM size and the name of the video card.
/// </summary>
/// <param name="p_adapter:"> The interface that has info of the video card.</param>
void Directx::Fill_VideoCard_Info(ComPtr<IDXGIAdapter> p_adapter) {
	using namespace Microsoft::WRL;
	DXGI_ADAPTER_DESC adapter_desc{};
	p_adapter->GetDesc(&adapter_desc);
	_memory = adapter_desc.DedicatedVideoMemory / 1024 / 1024;
	_description = adapter_desc.Description;
}
/// <summary>
/// The initialization of the depthstencil state.  
/// Only one reference here.
/// </summary>
void Directx::Initialize_DepthStencilState() {
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = Generate_DepthStencil_Desc();
	CHECK(_p_device->CreateDepthStencilState(&depth_stencil_desc, _p_depthstencil_state.ReleaseAndGetAddressOf()));
	_p_context->OMSetDepthStencilState(_p_depthstencil_state.Get(), 1);
}
/// <summary>
/// Initializing the Depth stencil view with the buffer.
/// </summary>
void Directx::Initialize_DepthStencilView() {
	using namespace Microsoft::WRL;
	ComPtr<ID3D11Texture2D> depth_stencil_buffer;
	D3D11_TEXTURE2D_DESC depth_stencil_desc = Generate_DepthStencilBuffer_Desc();
	CHECK(_p_device->CreateTexture2D(&depth_stencil_desc, NULL, depth_stencil_buffer.ReleaseAndGetAddressOf()));
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = Generate_DepthStencilView_Desc();
	CHECK(_p_device->CreateDepthStencilView(depth_stencil_buffer.Get(), &depth_stencil_view_desc, _p_depthstencil_view.ReleaseAndGetAddressOf()));
}
/// <summary>
/// Getting the actuell refresh rate of the monitor that close to 60hz.
/// </summary>
/// <param name="p_output:"> The interface that has info of the monitor.</param>
void Directx::Fill_Monitor_Info(ComPtr<IDXGIOutput> p_output) {
	DXGI_MODE_DESC mode_desc{};

	// the unscaled monitor resolution stores here. DEVMODE...
	// we do this to find the refresh rate like 59.9hz
	DEVMODE devmode{};
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
	mode_desc.Width = devmode.dmPelsWidth;
	mode_desc.Height = devmode.dmPelsHeight;
	mode_desc.RefreshRate = { 60, 1 };
	mode_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	CHECK(p_output->FindClosestMatchingMode(&mode_desc, &mode_desc, NULL));
	_refresh_rate = mode_desc.RefreshRate;
	//MessageBox(NULL, std::format(L"Refresh Rate: {}", _refresh_rate.Numerator / _refresh_rate.Denominator).c_str(), NULL, NULL);
}
/// <summary>
/// Getting the info of the monitor and video card if the program runs for the first time on this PC.
/// </summary>
void Directx::Initialize_Info() {
	using namespace Microsoft::WRL;
	using namespace std;
	ComPtr<IDXGIFactory> p_factory;
	ComPtr<IDXGIOutput> p_output;
	CHECK(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)p_factory.ReleaseAndGetAddressOf()));

	// loop checks the GPUs on the device and choose the one with the highest VRAM as a reference.
	for (int i = 0; ; i++) {
		ComPtr<IDXGIAdapter> p_candidate_adapter;
		if (FAILED(p_factory->EnumAdapters(i, p_candidate_adapter.ReleaseAndGetAddressOf()))) {
			break;
		}
		else {
			DXGI_ADAPTER_DESC adapter_desc{};
			p_candidate_adapter->GetDesc(&adapter_desc);
			//MessageBox(NULL, std::format(L"Memory: {} MB\nDescription: {}", adapter_desc.DedicatedVideoMemory / 1028 / 1028, adapter_desc.Description).c_str(), NULL, NULL);
			if (_memory < adapter_desc.DedicatedVideoMemory) {
				_memory = adapter_desc.DedicatedSystemMemory;
				_p_adapter = p_candidate_adapter;
			}
		}
	}

	// if the program runs on a laptop and the dedicated graphic card is not connecting to any monitor, then it will just pick the default 60hz, since the optimization is subtle.
	if (FAILED(_p_adapter->EnumOutputs(0, p_output.ReleaseAndGetAddressOf()))) {
		_refresh_rate = { 60, 1 };
		//MessageBox(NULL, L"Running on laptop?", L"Just asking", NULL);
	}
	else {
		Fill_Monitor_Info(p_output);
	}
	Fill_VideoCard_Info(_p_adapter);
}
/// <summary>
/// Rasterizer state.
/// </summary>
void Directx::Initialize_Rasterizer_State() {
	D3D11_RASTERIZER_DESC rasterizer_desc = Generate_Rasterizer_Desc();
	_p_device->CreateRasterizerState(&rasterizer_desc, _p_rasterizer_state.ReleaseAndGetAddressOf());
	_p_context->RSSetState(_p_rasterizer_state.Get());
}
/// <summary>
/// Create an interface to RenderTargetView that targeting the "back buffer", and save it as an attribute. Nothing special here.
/// </summary>
void Directx::Initialize_RenderTargetView() {
	using namespace Microsoft::WRL;
	HRESULT hr;
	ComPtr<ID3D11Texture2D> p_texture_2d;
	CHECK(_p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)p_texture_2d.ReleaseAndGetAddressOf()));
	CHECK(_p_device->CreateRenderTargetView(p_texture_2d.Get(), NULL, _p_render_target_view.ReleaseAndGetAddressOf()));
}
/// <summary>
/// Creating an interface to SwapChain, Device and DeviceContext. This is based upon the choosen video card. So make sure the "_p_adapter" is filled before calling this.
/// </summary>
void Directx::Initialize_SwapChain_Device_DeviceContext() {
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = Generate_SwapChain_Desc();
	CHECK(D3D11CreateDeviceAndSwapChain(_p_adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG, &feature_level, 1, D3D11_SDK_VERSION, &swap_chain_desc, _p_swap_chain.ReleaseAndGetAddressOf(), _p_device.ReleaseAndGetAddressOf(), NULL, _p_context.ReleaseAndGetAddressOf()));
}
/// <summary>
/// View port;
/// </summary>
void Directx::Initialize_ViewPort() {
	_viewport.Height = static_cast<float>(_window.GET_SIZE().cy);
	_viewport.MaxDepth = 1.f;
	_viewport.MinDepth = 0.f;
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_window.GET_SIZE().cx);
	_p_context->RSSetViewports(1, &_viewport);
}
/// <summary>
/// Nothing special here, just want to make a function for it.
/// </summary>
/// <returns>The description of a default swap chain.</returns>
DXGI_SWAP_CHAIN_DESC Directx::Generate_SwapChain_Desc() {
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.OutputWindow = _window.Get_HWND();
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = _window.GET_SIZE().cx;
	swap_chain_desc.BufferDesc.Height = _window.GET_SIZE().cy;
	swap_chain_desc.BufferDesc.RefreshRate = _refresh_rate;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Flags = NULL;
	return swap_chain_desc;
}
/// <summary>
/// Generating a default depth stencil description.
/// </summary>
/// <returns></returns>
D3D11_DEPTH_STENCIL_DESC Directx::Generate_DepthStencil_Desc() {
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilWriteMask = 0xff;
	depth_stencil_desc.StencilReadMask = 0xff;
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	D3D11_DEPTH_STENCILOP_DESC Front{};
	Front.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	Front.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Front.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Front.StencilFunc = D3D11_COMPARISON_ALWAYS;
	D3D11_DEPTH_STENCILOP_DESC Back = Front;
	Back.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.FrontFace = Front;
	depth_stencil_desc.BackFace = Back;
	return depth_stencil_desc;
}
/// <summary>
/// Depth stencil view description.
/// </summary>
/// <returns></returns>
D3D11_DEPTH_STENCIL_VIEW_DESC Directx::Generate_DepthStencilView_Desc() {
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	return depth_stencil_view_desc;
}
/// <summary>
/// Depth buffer description.
/// </summary>
/// <returns></returns>
D3D11_TEXTURE2D_DESC Directx::Generate_DepthStencilBuffer_Desc() {
	D3D11_TEXTURE2D_DESC texture_2d_desc{};
	texture_2d_desc.Width = _window.GET_SIZE().cx;
	texture_2d_desc.Height = _window.GET_SIZE().cy;
	texture_2d_desc.ArraySize = 1;
	texture_2d_desc.MipLevels = 1;
	texture_2d_desc.CPUAccessFlags = 0;
	texture_2d_desc.MiscFlags = 0;
	texture_2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture_2d_desc.SampleDesc.Count = 1;
	texture_2d_desc.SampleDesc.Quality = 0;
	texture_2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	return texture_2d_desc;
}
/// <summary>
/// The rasterizer description for the main scene.
/// </summary>
/// <returns></returns>
D3D11_RASTERIZER_DESC Directx::Generate_Rasterizer_Desc() {
	D3D11_RASTERIZER_DESC rasterizer_desc{};
	rasterizer_desc.AntialiasedLineEnable = false;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0.f;
	rasterizer_desc.DepthClipEnable = true;
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.FrontCounterClockwise = false;
	rasterizer_desc.MultisampleEnable = false;
	rasterizer_desc.ScissorEnable = false;
	rasterizer_desc.SlopeScaledDepthBias = 0.f;
	return rasterizer_desc;
}

