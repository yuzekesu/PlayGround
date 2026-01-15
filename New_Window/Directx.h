#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Window.h"
#include <d3d11.h>
#include <dxgi.h>
#include <dxgicommon.h>
#include <string>
#include <Windows.h>
#include <wrl/client.h>

/// <summary>
/// The graphic of the program. Of course the "window" itself is apart of the graphic. Thus it has window as its attribute.
/// </summary>
class Directx {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Directx();
	ComPtr<IDXGISwapChain> Get_SwapChain();
	ComPtr<ID3D11DeviceContext> Get_Context();
	ComPtr<ID3D11DepthStencilView> Get_DepthStencilView();
	ComPtr<ID3D11RenderTargetView> Get_RenderTargetView();
	float Get_Refresh_Rate() const;
private:
	void Fill_Monitor_Info(ComPtr<IDXGIOutput> p_output);
	void Fill_VideoCard_Info(ComPtr<IDXGIAdapter> p_adapter);
	void Initialize_DepthStencilState();
	void Initialize_DepthStencilView();
	void Initialize_Info();
	void Initialize_Rasterizer_State();
	void Initialize_RenderTargetView();
	void Initialize_SwapChain_Device_DeviceContext();
	void Initialize_ViewPort();
	DXGI_SWAP_CHAIN_DESC Generate_SwapChain_Desc();
	D3D11_DEPTH_STENCIL_DESC Generate_DepthStencil_Desc();
	D3D11_DEPTH_STENCIL_VIEW_DESC Generate_DepthStencilView_Desc();
	D3D11_TEXTURE2D_DESC Generate_DepthStencilBuffer_Desc();
	D3D11_RASTERIZER_DESC Generate_Rasterizer_Desc();
private:
	bool _vsync; // unused
	std::wstring _description;
	DXGI_RATIONAL _refresh_rate;
	D3D11_VIEWPORT _viewport;
	ComPtr<IDXGIAdapter> _p_adapter;
	ComPtr<IDXGISwapChain> _p_swap_chain;
	ComPtr<ID3D11DepthStencilState> _p_depthstencil_state;
	ComPtr<ID3D11DepthStencilView> _p_depthstencil_view;
	ComPtr<ID3D11Device> _p_device;
	ComPtr<ID3D11DeviceContext> _p_context;
	ComPtr<ID3D11RenderTargetView> _p_render_target_view;
	ComPtr<ID3D11Texture2D> _p_depthstencil_buffer;
	ComPtr<ID3D11RasterizerState> _p_rasterizer_state;
	SIZE_T _memory = 0;
	Window _window;
};
