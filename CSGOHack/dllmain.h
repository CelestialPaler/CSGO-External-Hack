#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mutex>

#include "Ref/Indicium/Engine/IndiciumCore.h"
#include "Ref/Indicium/Engine/IndiciumDirect3D9.h"
#include "Ref/Indicium/Engine/IndiciumDirect3D11.h"

#include "HUD/HUD.h"
#include "FontWrapper.h"

typedef LRESULT(WINAPI* t_WindowProc)(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);
