#ifndef VERTEX_BUFFER_H__
#define VERTEX_BUFFER_H__

#include <d3d11.h>
#include <wrl/client.h>

template<class T>
class VertexBuffer
{
private:
	VertexBuffer(const VertexBuffer<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize = 0u;

public:
	VertexBuffer() {}

	ID3D11Buffer* Get() const 
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer.GetAddressOf();

	}


	UINT BufferSize() const
	{
		return this->bufferSize;
	}

	const UINT Stride() const
	{
		return *this->stride.get();
	}
	const UINT* StridePtr() const
	{
		return this->stride.get();
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertices)
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}
		this->bufferSize = numVertices;
		if (stride.get() == nullptr)
			this->stride = std::make_unique<UINT>((UINT)sizeof(T));

		D3D11_BUFFER_DESC vertexBufferDesc = {};

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * numVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData = {};
		vertexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, buffer.GetAddressOf());
		return hr;
	}



};

#endif