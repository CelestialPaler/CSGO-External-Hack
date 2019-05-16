// CTHackFramework 														      
//	A framework for general game hacking								      
// Copyright © 2019 Celestial Tech All rights reserved.
//
// The MIT License (MIT)
// Copyright (c) 2019 Celestial Tech
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
	// 清空屏幕
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