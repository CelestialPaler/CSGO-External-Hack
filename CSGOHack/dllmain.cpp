﻿/*
MIT License

Copyright (c) 2018 Benjamin H鰃linger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "DirectX/Graphics.h"
#include "HUD/HUD.h"
#include "Ref/Indicium/Engine/IndiciumCore.h"
#include "Ref/Indicium/Engine/IndiciumDirect3D9.h"
#include "Ref/Indicium/Engine/IndiciumDirect3D11.h"

#ifdef _M_IX86
#pragma comment(lib, "x86\\FW1FontWrapper.lib")
#else
#pragma comment(lib, "x64\\FW1FontWrapper.lib")
#endif
#pragma comment(lib, "dxguid.lib")


PINDICIUM_ENGINE engine = nullptr;
ID3D11DeviceContext* ctx = nullptr;
ID3D11Device* dev = nullptr;

EVT_INDICIUM_D3D9_PRESENT EvtIndiciumD3D9Present;


/**
 * \fn  BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
 *
 * \brief   Your typical DLL entry point function. We're not doing much here since a special
 *          initialization routine gets called upon getting loaded by Indicium-Supra.
 *
 * \author  Benjamin "Nefarius" H鰃linger
 * \date    05.05.2018
 *
 * \param   hInstance   The instance.
 * \param   dwReason    The reason.
 * \param   parameter3  The third parameter.
 *
 * \return  A WINAPI.
 */
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	//
	// We don't need to get notified in thread attach- or detachments
	// 
	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));

	INDICIUM_D3D9_EVENT_CALLBACKS d3d9;
	INDICIUM_D3D9_EVENT_CALLBACKS_INIT(&d3d9);
	d3d9.EvtIndiciumD3D9PrePresent = EvtIndiciumD3D9Present;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		if (!engine)
		{
			engine = IndiciumEngineAlloc();

			IndiciumEngineSetD3D9EventCallbacks(engine, &d3d9);

			(void)IndiciumEngineInit(engine, nullptr);
		}

		break;
	case DLL_PROCESS_DETACH:

		if (engine)
		{
			IndiciumEngineShutdown(engine, nullptr);
			IndiciumEngineFree(engine);
			engine = nullptr;
		}

		break;
	default:
		break;
	}

	return TRUE;
}



Graphics gfx;
void EvtIndiciumD3D9Present(
	IDXGISwapChain* pSwapChain,
	UINT            SyncInterval,
	UINT            Flags
)
{
	D3D11_DEVICE_CONTEXT_FROM_SWAPCHAIN(pSwapChain, &dev, &ctx);

	gfx.DrawStr(L"Copyright (c) 2019 Celestial Tech All rights reserved.", { 500, 500 }, DirectX::XMFLOAT3({0,0,0}));
}
