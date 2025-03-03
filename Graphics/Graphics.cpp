#include "Graphics.h"


static float pushFloatSensibilityMenu(const char* title, float initial_sensibility) {
	ImGui::PushID(title);
	ImGuiStorage* s = ImGui::GetStateStorage();
	float sensibility = s->GetFloat(ImGui::GetID("sensibility"), initial_sensibility);
	return sensibility;
}

static void popFloatSensibilityMenu(float sensibility) {
	bool mouse_becomes_down = ImGui::IsMouseClicked(2);
	if (ImGui::IsItemHovered()) {
		if (mouse_becomes_down)
			ImGui::OpenPopup("dragfloatex_options");
	}
	if (ImGui::BeginPopup("dragfloatex_options")) {
		if (ImGui::MenuItem("0.001", "", sensibility == 0.001f))
			sensibility = 0.001f;
		if (ImGui::MenuItem("0.01", "", sensibility == 0.01f))
			sensibility = 0.01f;
		if (ImGui::MenuItem("0.1", "", sensibility == 0.1f))
			sensibility = 0.1f;
		if (ImGui::MenuItem("1", "", sensibility == 1.0f))
			sensibility = 1.0f;
		if (ImGui::MenuItem("10", "", sensibility == 10.0f))
			sensibility = 10.0f;
		if (ImGui::MenuItem("100", "", sensibility == 100.0f))
			sensibility = 100.0f;
		if (ImGui::MenuItem("1000", "", sensibility == 1000.0f))
			sensibility = 1000.0f;
		ImGui::EndPopup();
	}
	ImGuiStorage* s = ImGui::GetStateStorage();
	s->SetFloat(ImGui::GetID("sensibility"), sensibility);
	ImGui::PopID();
}


void Graphics::RenderFrame()
{


	////textured square
	//Vertex v[] = {
	//	Vertex(-0.5f,-0.5f, 0.f, 0.0f, 1.0f), //Bottom Left Point
	//	Vertex(-0.5f,0.5f,0.f, 0.0f, 0.0f), //Top Left Point
	//	Vertex(0.5f,0.5f,0.f, 1.0f, 0.0f), //Top Right Point
	//	Vertex(0.5f,-0.5f,0.f,1.0f,1.0f), //Bottom Right Point
	//};

	//HRESULT hr = this->vertexBuffer.Initialize(device.Get(), v, (UINT)ARRAYSIZE(v));
	//if (FAILED(hr))
	//{
	//	ErrorLogger::Log(hr, "Failed to create vertex buffer.");
	//	return;
	//}


	/*DWORD indices[] =
	{
		0,1,2,
		0,2,3
	};

	HRESULT hr = this->indexBuffer.Initialize(device.Get(), indices, (UINT)ARRAYSIZE(indices));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create index buffer.");
		return;
	}*/


	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	UINT offset = 0;

	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	deviceContext->OMGetDepthStencilState(depthStencilState.GetAddressOf(), 0);
	this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF);
	deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);
	
	static float translationOffset[3] = { 0.f,0.f,0.f };


	
	{ //Pink Texture
		//Update Constant Buffer
		static float translationOffset[3] = { 0, 0, 5.0f };
		XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
		cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
		cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

		if (!cb_vs_vertexshader.ApplyChanges())
			return;
		this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

		this->cb_ps_pixelshader.data.alpha = 1.0f;
		this->cb_ps_pixelshader.ApplyChanges();
		this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

		//Square
		this->deviceContext->PSSetShaderResources(0, 1, this->grassTexture.GetAddressOf());
		this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
		this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		this->deviceContext->RSSetState(this->rasterizerState.Get());
		this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
	}
	
	static float alpha = 1.0f;

	{ //Pink Texture
		//Update Constant Buffer
		static float translationOffset2[3] = { 0, 0, -1.0f };
		XMMATRIX world = XMMatrixTranslation(translationOffset2[0], translationOffset2[1], translationOffset2[2]);
		cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
		cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

		if (!cb_vs_vertexshader.ApplyChanges())
			return;
		this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

		this->cb_ps_pixelshader.data.alpha = alpha;
		this->cb_ps_pixelshader.ApplyChanges();
		this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

		//Square
		this->deviceContext->PSSetShaderResources(0, 1, this->pavementTexture.GetAddressOf());
		this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
		this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		this->deviceContext->RSSetState(this->rasterizerState_CullFront.Get());
		this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
		this->deviceContext->RSSetState(this->rasterizerState.Get());
		this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
	}



	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if ((float)fpsTimer.GetMilisecondsElapsed() > 1000.0f)
	{
		fpsString = "FPS:" + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0.f, 0.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT2(1.f, 1.f));
	spriteBatch->End();
	static int counter = 0;
	// IMGUI STUFF
	static int ival = 100;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//Create ImGui Test Window
	ImGui::Begin("Test");
	static bool view_is_local = false;
	if (ImGui::Button("Count"))
	{
		counter += 1;
		if (counter % 2 == 0) // even, change words
		{
			view_is_local = true;
		}
		else
		{
			view_is_local = false;
		}

		//if (ImGui::Button(view_is_local ? "Local###Select" : "Global###Deselect"))
			//view_is_local = !view_is_local;
	}
	ImGui::SameLine();
	std::string clickCount = "->  " + std::to_string(counter) + "\n";
	ImGui::Text(clickCount.c_str());
	if (ImGui::Button(view_is_local ? "Local###Select" : "Global###Deselect"))
		view_is_local = !view_is_local;


//	ImGui::SameLine();
	//ImGui::DragFloat3("Translation  X / Y / Z: ", translationOffset, 1.0f, 0.0f,-5.f, 0);
	ImGui::DragFloat3Ex(": Translation", translationOffset, 0.001f, -30.f, 999.9f);
	ImGui::DragFloatEx("Alpha", &alpha, 0.001f, 0.0f, 1.0f);

	//popFloatSensibilityMenu(1.0f);
	ImGui::End();

	// assemble together draw dataa
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());




	this->swapchain->Present(0, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = windowWidth;
	scd.BufferDesc.Height = windowHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(	adapters[0].pAdapter, //IDXGI Adapter
										D3D_DRIVER_TYPE_UNKNOWN,
										NULL, //FOR SOFTWARE DRIVER TYPE
										NULL, //FLAGS FOR RUNTIME LAYERS
										NULL, //FEATURE LEVELS ARRAY
										0, //# OF FEATURE LEVELS IN ARRAY
										D3D11_SDK_VERSION,
										&scd, //Swapchain description
										this->swapchain.GetAddressOf(), //Swapchain Address
										this->device.GetAddressOf(), //Device Address
										NULL, //Supported feature level
										this->deviceContext.GetAddressOf()); //Device Context Address

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "GetBuffer Failed.");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = windowWidth;
	depthStencilDesc.Height = windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil buffer");
		return false;
	}

	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil view");
		return false;
	}


	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), depthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateDepthStencilState(&depthStencilStateDesc, depthStencilState.GetAddressOf());
	if (hr != S_OK)
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil state");
		return false;
	}


	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)windowWidth;
	viewport.Height = (FLOAT)windowHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc = {};

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	hr = this->device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
	if (hr != S_OK)
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state");
		return false;
	}

	D3D11_RASTERIZER_DESC rasterizerDesc_CullFront = {};

	rasterizerDesc_CullFront.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, rasterizerState_CullFront.GetAddressOf());
	if (hr != S_OK)
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state");
		return false;
	}

	//// Create Blend State
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.BlendEnable = true;
	bd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.BlendOp = D3D11_BLEND_OP_ADD;
	bd.SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	//blendDesc.AlphaToCoverageEnable = true;
	//blendDesc.IndependentBlendEnable = true;
	blendDesc.RenderTarget[0] = bd;

	hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create blend state");
		return false;
	}

	//


	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comicSansMS16.spritefont");

	// create sampler state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create sampler state");
		return false;
	}



	return true;
}

bool Graphics::InitializeShaders()
{

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"Shaders\\Bytecode\\";
	#else  //x86 (Win32)
			shaderfolder = L"Shaders\\Bytecode\\";
	#endif
	#else //Release Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"Shaders\\Bytecode\\";
	#else  //x86 (Win32)
			shaderfolder = L"Shaders\\Bytecode\\";
	#endif
#endif
	}


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexshader.Initialize(this->device, shaderfolder + L"vertexshader.cso", layout, numElements))
		return false;

	if (!pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso"))
		return false;


	return true;
}

bool Graphics::InitializeScene()
{
	//Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f), //Bottom Left   - [0]
		Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f), //Top Left      - [1]         - FRONT
		Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f), //Top Right     - [2]
		Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f), //Bottom Right   - [3]

		Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f), //Bottom Left   - [4]
		Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f), //Top Left      - [5]         - BACK
		Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f), //Top Right     - [6]
		Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f), //Bottom Right   - [7]
	};

	//Load Vertex Data
	HRESULT hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}

	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3, // front
		4,7,6,
		4,6,5, // back
		3,2,6,
		3,6,7, // right
		4,5,1,
		4,1,0, // left
		1,5,6,
		1,6,2, // top
		0,3,7,
		0,7,4 // bottom
	};

	//Load Index Data

	hr = this->indexBuffer.Initialize(this->device.Get(), indices, ARRAYSIZE(indices));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create indices buffer.");
		return hr;
	}

	//Load Texture
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create wic texture from file.");
		return false;
	}
	
	//Load Texture
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pink.bmp", nullptr, pinkTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create wic texture from file.");
		return false;
	}

	//Load Texture
	hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_pavement.jpg", nullptr, pavementTexture.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create wic texture from file.");
		return false;
	}


	//Initialize Constant Buffer(s)
	hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
		return false;
	}

	hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
		return false;
	}


	camera.SetPosition(0.f, 0.f, -2.f);
	camera.SetProjectionValues(90.f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.f);

	return true;
}


bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	fpsTimer.Start();

	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}


	

	bool ImGui::DragFloatEx(const char* title, float* addr_value, float initial_sensibility, float v_min, float v_max) {
		float sensibility = pushFloatSensibilityMenu(title, initial_sensibility);
		bool changed = ImGui::DragFloat(title, addr_value, sensibility, v_min, v_max);
		popFloatSensibilityMenu(sensibility);
		return changed;
	}

	bool ImGui::DragFloat2Ex(const char* title, float v[2], float initial_sensibility, float v_min, float v_max) {
		float sensibility = pushFloatSensibilityMenu(title, initial_sensibility);
		bool changed = ImGui::DragFloat2(title, v, sensibility, v_min, v_max);
		popFloatSensibilityMenu(sensibility);
		return changed;
	}

	bool ImGui::DragFloat3Ex(const char* title, float v[3], float initial_sensibility, float v_min, float v_max) {
		float sensibility = pushFloatSensibilityMenu(title, initial_sensibility);
		bool changed = ImGui::DragFloat3(title, v, sensibility, v_min, v_max);
		popFloatSensibilityMenu(sensibility);
		return changed;
	}

	bool ImGui::DragFloat4Ex(const char* title, float v[4], float initial_sensibility, float v_min, float v_max) {
		float sensibility = pushFloatSensibilityMenu(title, initial_sensibility);
		bool changed = ImGui::DragFloat4(title, v, sensibility, v_min, v_max);
		popFloatSensibilityMenu(sensibility);
		return changed;
	}

	bool ImGui::DragIntEx(const char* title, int* addr_value, float speed, int v_min, int v_max) {
		ImGui::PushID(title);
		ImGuiStorage* s = ImGui::GetStateStorage();
		int sensibility = s->GetInt(ImGui::GetID("sensibility"), 1);
		bool changed = ImGui::DragInt(title, addr_value, sensibility * speed, v_min, v_max);
		bool mouse_becomes_down = ImGui::IsMouseClicked(2);
		if (ImGui::IsItemHovered()) {
			if (mouse_becomes_down)
				ImGui::OpenPopup("dragintex_options");
		}
		if (ImGui::BeginPopup("dragintex_options")) {
			if (ImGui::MenuItem("1", "", sensibility == 1))
				sensibility = 1;
			if (ImGui::MenuItem("10", "", sensibility == 10))
				sensibility = 10;
			if (ImGui::MenuItem("100", "", sensibility == 100))
				sensibility = 100;
			if (ImGui::MenuItem("1000", "", sensibility == 1000))
				sensibility = 1000;
			ImGui::EndPopup();
		}
		s->SetInt(ImGui::GetID("sensibility"), sensibility);
		ImGui::PopID();
		return changed;
	}
