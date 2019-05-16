#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
		return false;

	if (!InitializeShaders())
		return false;
	return true;
}

void Graphics::ClearFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// 绘制矩形框
bool Graphics::DrawRect(int x1, int y1, int x2, int y2, DirectX::XMFLOAT3 color, int w, int h)
{
	// 取得Logger
	//Util::Logger* logger = Util::Logger::GetInstance();

	// 转换为屏幕百分比 （-1.0，1.0）
	float x1Persentage = ((float)x1 / w - 0.5) * 2;
	float y1Persentage = ((float)y1 / h - 0.5) * 2;
	float x2Persentage = ((float)x2 / w - 0.5) * 2;
	float y2Persentage = ((float)y2 / h - 0.5) * 2;

	// 顶点
	Vertex vertexts[] =
	{
		Vertex(x1Persentage, y1Persentage, 1.0f, color.x, color.y, color.z),
		Vertex(x1Persentage, y2Persentage, 1.0f,color.x, color.y, color.z),
		Vertex(x2Persentage, y2Persentage, 1.0f,color.x, color.y, color.z),
		Vertex(x2Persentage, y1Persentage, 1.0f,color.x, color.y, color.z),
		Vertex(x1Persentage, y1Persentage, 1.0f, color.x, color.y, color.z)
	};

	// D3D缓冲设置
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertexts);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertexts;

	// 创建D3D缓冲区
	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		//{
		//	Util::Logger::LogDisc msg;
		//	msg.emPriority = Util::Logger::Priority::ERR;
		//	msg.szFrom = __func__;
		//	msg.szMsg = "创建DirectX缓冲失败";
		//	logger->Write(msg);
		//}
		return false;
	}

	// DC设置
	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// 绘制顶点
	this->deviceContext->Draw(5, 0);
	this->vertexBuffer->Release();
	return true;
}

bool Graphics::DrawStr(std::wstring str, DirectX::XMFLOAT2 coord, DirectX::XMFLOAT3 color)
{
	const DirectX::XMVECTORF32 c = { {{color.x,color.y,color.z,1.0f}} };
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), str.c_str(), coord, c, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();
	return false;
}

bool Graphics::DrawLine(int x1, int y1, int x2, int y2, DirectX::XMFLOAT3 color, int w, int h)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	float _x1 = ((float)x1 / w - 0.5) * 2;
	float _y1 = ((float)y1 / h - 0.5) * 2;
	float _x2 = ((float)x2 / w - 0.5) * 2;
	float _y2 = ((float)y2 / h - 0.5) * 2;
	Vertex v[] =
	{
		Vertex(_x1, _y1, 1.0f, color.x, color.y, color.z),
		Vertex(_x2, _y2, 1.0f,color.x, color.y, color.z),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create vertex buffer.";
			logger->Write(msg);
		}
		return false;
	}

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	this->deviceContext->Draw(2, 0);
	this->vertexBuffer->Release();
	return true;
}

bool Graphics::DrawCircle(int x, int y, int rad, int count, DirectX::XMFLOAT3 color, int w, int h)
{
	return true;
}

void Graphics::RenderFrame()
{
	this->swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "No IDXGI Adapters found.";
			logger->Write(msg);
		}
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
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
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create device and swapchain.";
			logger->Write(msg);
		}
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) //If error occurred
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "GetBuffer Failed.";
			logger->Write(msg);
		}
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create render target view.";
			logger->Write(msg);
		}
		return false;
	}

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
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
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create depth stencil buffer.";
			logger->Write(msg);
		}
		return false;
	}

	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create depth stencil view.";
			logger->Write(msg);
		}
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create depth stencil state.";
			logger->Write(msg);
		}
		return false;
	}

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	this->deviceContext->RSSetViewports(1, &viewport);

	//Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create rasterizer state.";
			logger->Write(msg);
		}
		return false;
	}

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"courier16.spritefont");

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
			shaderfolder = L"Shader\\";
	#else  //x86 (Win32)
			shaderfolder = L"Shader\\";
	#endif
	#else //Release Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"Shader\\";
	#else  //x86 (Win32)
			shaderfolder = L"Shader\\";
	#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexshader.Initialize(this->device, shaderfolder + L"VertexShader.cso", layout, numElements))
		return false;

	if (!pixelshader.Initialize(this->device, shaderfolder + L"PixelShader.cso"))
		return false;


	return true;
}