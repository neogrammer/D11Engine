#include "Graphics.h"
#include <vector>
#include "AdapterReader.h"

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
    std::vector<AdapterData> adapters = AdapterReader::getAdapterData();

    if (adapters.size() < 1)
    {
        ErrorLogger::Log("No DXGI Adapters found.");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount=1;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.OutputWindow = hwnd;
    scd.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
    scd.Windowed = TRUE;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;



    HRESULT hr;
    hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, D3D_DRIVER_TYPE_UNKNOWN,
        NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &scd,
        this->swapchain.GetAddressOf(), this->device.GetAddressOf(), NULL, this->deviceContext.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log("SwapChain Not properly created!");
        return false;
    }
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr))
    {
        ErrorLogger::Log("GetBuffer failed");
        return false;
    }
    

    hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log("Failed to create render target view");
        return false;
    }

    this->deviceContext->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), NULL);

    return true;
}

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    if (!InitializeDirectX(hwnd, width, height))
        return false;

    return true;
}

void Graphics::RenderFrame()
{
    float bgcolor[] = {.16f,.65f,.95f,1.f};
    this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
    this->swapchain->Present(1, NULL);

}
