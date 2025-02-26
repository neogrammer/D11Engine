#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class Graphics
{
	bool InitializeDirectX(HWND hwnd, int width, int height);

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
};