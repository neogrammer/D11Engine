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
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	UINT offset = 0;
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	auto* dstate = depthStencilState.Get();
	deviceContext->OMGetDepthStencilState(&dstate, 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);


	//static float translationOffset[3] = { 0.f,0.f,0.f };
	//{ //Pink Texture
	//	//Update Constant Buffer
	//	static float translationOffset[3] = { 0, 0, 5.0f };
	//	XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
	//	cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	//	cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);
	//	if (!cb_vs_vertexshader.ApplyChanges())
	//		return;
	//	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());
	//	this->cb_ps_pixelshader.data.alpha = 1.0f;
	//	this->cb_ps_pixelshader.ApplyChanges();
	//	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());
	//	//Square
	//	this->deviceContext->PSSetShaderResources(0, 1, this->grassTexture.GetAddressOf());
	//	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	//	this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//	this->deviceContext->RSSetState(this->rasterizerState.Get());
	//	this->deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
	//}

	//{ //Pink Texture
		//Update Constant Buffer
		
		//XMMATRIX world = XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
	/*	cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
		cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);
		if (!cb_vs_vertexshader.ApplyChanges())
			return;
		this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());*/

		this->gameObject.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	/*	this->cb_ps_pixelshader.data.alpha = 1.f;*/
		/*this->cb_ps_pixelshader.ApplyChanges();
		this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());*/
		//Square
		/*this->deviceContext->PSSetShaderResources(0, 1, this->pavementTexture.GetAddressOf());
		this->deviceContext->IASetVertexBuffers(0, 1, model.getVertexBuffer().GetAddressOf(), model.getVertexBuffer().StridePtr(), &offset);
		this->deviceContext->IASetIndexBuffer(model.getIndexBuffer()->Get(), DXGI_FORMAT_R32_UINT, 0u);
		this->deviceContext->RSSetState(this->rasterizerState_CullFront.Get());
		this->deviceContext->DrawIndexed(model.getIndexBuffer()->BufferSize(), 0u, 0u);
		this->deviceContext->RSSetState(this->rasterizerState.Get());
		this->deviceContext->DrawIndexed(model.getIndexBuffer()->BufferSize(), 0u, 0u);*/
	//}
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
	}
	ImGui::SameLine();
	std::string clickCount = "->  " + std::to_string(counter) + "\n";
	ImGui::Text(clickCount.c_str());
	if (ImGui::Button(view_is_local ? "Local###Select" : "Global###Deselect"))
		view_is_local = !view_is_local;
//	ImGui::SameLine();
	//ImGui::DragFloat3Ex(": Translation", translationOffset, 0.001f, -30.f, 999.9f);
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
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}
		DXGI_SWAP_CHAIN_DESC scd = { 0 };
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
		UINT creationFlags = D3D11_CREATE_DEVICE_DEBUG;
		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, //IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, //FOR SOFTWARE DRIVER TYPE
			creationFlags, //FLAGS FOR RUNTIME LAYERS
			NULL, //FEATURE LEVELS ARRAY
			0, //# OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION,
			&scd, //Swapchain description
			this->swapchain.GetAddressOf(), //Swapchain Address
			this->device.GetAddressOf(), //Device Address
			NULL, //Supported feature level
			this->deviceContext.GetAddressOf()); //Device Context Address
		comfail(hr, "Failed to create device, device context and swapchain");
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		comfail(hr, "Failed to GetBuffer()");
		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		comfail(hr, "Failed to create render target view");
		CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT,this->windowWidth, this->windowHeight);
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		comfail(hr, "Failed to create depth stencil buffer");
		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
		comfail(hr, "Failed to create  depth stencil view");
		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), depthStencilView.Get());
		CD3D11_DEPTH_STENCIL_DESC dssd(D3D11_DEFAULT);
		dssd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = device->CreateDepthStencilState(&dssd, depthStencilState.GetAddressOf());
		comfail(hr, "Failed to create  depth stencil state");
		//Create the Viewport
		CD3D11_VIEWPORT viewport(0.f, 0.f, (float)windowWidth, (float)windowHeight);
		//Set the Viewport
		this->deviceContext->RSSetViewports(1, &viewport);
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		hr = this->device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
		comfail(hr, "Failed to create  rasterizer state Cull Back");
		CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, rasterizerState_CullFront.GetAddressOf());
		comfail(hr, "Failed to create  rasterizer state Cull Front");
		//// Create Blend State
		D3D11_BLEND_DESC blendDesc = { 0 };
		D3D11_RENDER_TARGET_BLEND_DESC bd = { 0 };
		bd.BlendEnable = true;
		bd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.BlendOp = D3D11_BLEND_OP_ADD;
		bd.SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[0] = bd;
		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		comfail(hr, "Failed to create blend state");
		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comicSansMS16.spritefont");
		// create sampler state
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());
		comfail(hr, "Failed to create the sampler state");
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
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
	try
	{

		HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
		comfail(hr, "Failed to initialize grass texture");
		//Load Texture
		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pink.bmp", nullptr, pinkTexture.GetAddressOf());
		comfail(hr, "Failed to initialize Pink texture");
		//Load Texture
		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Objects\\nanosuit\\NanoSuit_us.dds", nullptr, pavementTexture.GetAddressOf());
		comfail(hr, "Failed to initialize pavement texture");
		//Initialize Constant Buffer(s)
		hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		comfail(hr, "Failed to initialize constant buffer for vertex shader");
		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		comfail(hr, "Failed to initialize constant buffer for pixel shader");


		// Initialize models
		gameObject.Initialize("Data\\Objects\\nanosuit\\Nanosuit.obj", this->device.Get(), this->deviceContext.Get(), this->pavementTexture.Get(), cb_vs_vertexshader);
	/*		ErrorLogger::Log(S_OK, L"Model initialization error!");
			return false;
		}*/
		gameObject.SetRotation(DirectX::XM_PIDIV2, DirectX::XM_PI, 0.f);
		gameObject.SetPosition(0.f, -1.3f, 2.f);
		//model.SetPosition(0.f, 0.f, 5.f);
		camera.SetProjectionValues(90.f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.f);
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
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