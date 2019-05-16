#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "..//Ref/DirectxTK/Include/SpriteBatch.h"
#include "..//Ref/DirectxTK/Include/SpriteFont.h"

class Graphics
{
public:
	// 初始化函数
	bool Initialize(HWND hwnd, int width, int height);
	// 刷新（清空）屏幕
	void ClearFrame();
	// 绘制窗口
	void RenderFrame();

public:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool InitializeShaders();

public:
	// 绘制矩形框
	bool DrawRect(int x1, int y1, int x2, int y2, DirectX::XMFLOAT3 color, int w, int h);
	bool DrawStr(std::wstring str, DirectX::XMFLOAT2 coord, DirectX::XMFLOAT3 color);
	bool DrawLine(int x1, int y1, int x2, int y2, DirectX::XMFLOAT3 color, int w, int h);
	bool DrawCircle(int x, int y, int rad, int count, DirectX::XMFLOAT3 color, int w, int h);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexshader;
	PixelShader pixelshader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
};