#include "Graphic.h"
#include "NormalizedColor.h"
#include <d3d11.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

float Graphic::Get_Refresh_Rate() {
	return _direct_x.Get_Refresh_Rate();
}

void Graphic::Begin_Scene(NormalizedColor normalized_color)
{
	ComPtr<ID3D11DeviceContext> dc = _direct_x.Get_Context();
	ComPtr<ID3D11RenderTargetView> rtv = _direct_x.Get_RenderTargetView();
	ComPtr<ID3D11DepthStencilView> dsv = _direct_x.Get_DepthStencilView();
	dc->ClearRenderTargetView(rtv.Get(), normalized_color.data());
	dc->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void Graphic::End_Scene()
{
	_direct_x.Get_SwapChain()->Present(1, 0);
}
