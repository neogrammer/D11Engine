#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath)
{
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->shader_buffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring errorMsg = L"Failed to load shader: ";
        errorMsg += shaderPath;
        ErrorLogger::Log(hr, errorMsg);
        return false;
    }

    hr = device->CreateVertexShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, shader.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring errorMsg = L"Failed to create vertex shader: ";
        errorMsg += shaderPath;
        ErrorLogger::Log(hr, errorMsg);
        return false;
    }
    return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
    return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
    return this->shader_buffer.Get();
}
