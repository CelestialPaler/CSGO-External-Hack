﻿// CTHackFramework 														      
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

#include "HUD.h"

void HUD::DrawRect(int x, int y, int w, int h, int stroke, DirectX::XMFLOAT3 color)
{
	for (size_t i = 0; i < stroke; i++)
	{
		graphics->DrawRect(x - w / 2 + i, y - h / 2 + i, x + w / 2 - i, y + h / 2 - i, color, targetWndWidth, targetWndHeight);
	}
}

void HUD::DrawString(std::wstring str, int x, int y, DirectX::XMFLOAT3 color)
{
	graphics->DrawStr(str, DirectX::XMFLOAT2(x, y), color);
}

void HUD::DrawLine(int x1, int y1, int x2, int y2, int stroke, DirectX::XMFLOAT3 color)
{
	for (size_t i = 0; i < 2 * stroke; i++)
	{
		graphics->DrawLine(x1 + i - stroke, y1, x2, y2, color, targetWndWidth, targetWndHeight);
	}
}

void HUD::TimerStart(void)
{
	start = std::chrono::system_clock::now();
}

void HUD::TimerStop()
{
	end = std::chrono::system_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	timeDelta.push_back(duration.count());
	if (timeDelta.size() > smoothIndex)
	{
		timeDelta.erase(timeDelta.begin());
	}

	int tempSum = 0;
	for (size_t i = 0; i < timeDelta.size(); i++) tempSum += timeDelta.at(i);
	smoothDelta = (float)tempSum / (float)timeDelta.size();
}

static float fps = 0;
static int frameCount = 0;
static int currentTime = 0;
static int lastTime = 0;
float HUD::GetFPS()
{
	frameCount++;
	currentTime = (int)timeGetTime();
	if (currentTime - lastTime >= 1000)
	{
		fps = (float)(frameCount * 1000) / (float)(currentTime - lastTime);
		lastTime = currentTime;
		frameCount = 0;
	}
	return fps;
}

void HUD::ShowDeclare(int x, int y)
{
	static char temp[] = "";
	DrawString(L"CSGO External Hack Demo", x, y, Red);
	DrawString(L"Copyright (c) 2019 Celestial Tech All rights reserved.", x, y + 20, Red);
	DrawString(L"Development Build : " + devVersion, x, y + 40, Red);
}

DX::StepTimer s_timer;

void StepTimerUpdate(DX::StepTimer const& timer)
{
	float delta = float(timer.GetElapsedSeconds());
}

void Tick()
{
	s_timer.Tick([&]()
		{
			StepTimerUpdate(s_timer);
		});
}


void HUD::ShowTimeDebug(int x, int y)
{
	Tick();

	std::stringstream ss;
	ss << "Performance Debug Info: ";
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y, Red);
	ss.str("");

	ss << "UTDelta: " << std::setprecision(3) << smoothDelta << std::setfill(' ');
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 20, Red);
	ss.str("");

	int fps = (int)GetFPS();
	ss << "FPS/UPS: " << std::dec << fps << "/" << (int)(1000 / smoothDelta);
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 40, Red);
	ss.str("");

	ss << "CT/LT: " << currentTime << "/" << lastTime;
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 60, Red);
	ss.str("");

	ss << "Local FPS/FC: " << fps << "/" << frameCount;
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 80, Red);
	ss.str("");

	ss << "Step FPS: " << s_timer.GetFramesPerSecond();
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 100, Red);
	ss.str("");
}

void HUD::ShowFunctionInfo(int x, int y)
{
	std::stringstream ss;
	ss << "F1 - Overlay : " << (FunctionEnableFlag::bOverlay ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 0, Red);
	ss.str("");

	ss << "F2 - TriggerBot : " << (FunctionEnableFlag::bTriggerBot ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 20, Red);
	ss.str("");

	ss << "F3 - AimBot : " << (FunctionEnableFlag::bNULL ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 40, Red);
	ss.str("");

	ss << "F4 - ESP : " << (FunctionEnableFlag::bESP ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 60, Red);
	ss.str("");

	ss << "F5 - RadarHack : " << (FunctionEnableFlag::bNULL ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 80, Red);
	ss.str("");

	ss << "F6 - BHop : " << (FunctionEnableFlag::bNULL ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 100, Red);
	ss.str("");

	ss << "F7 - Glow : " << (FunctionEnableFlag::bNULL ? "ON" : "OFF");
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 120, Red);
	ss.str("");
}

void HUD::ShowESPDebug(int x, int y)
{
	std::stringstream ss;
	ss << "ESP Debug Info: ";
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y, Red);
	ss.str("");

	int teammatesNum = 0, enemyNum = 0;
	int teammatesLiveNum = 0, enemyLiveNum = 0;
	for (size_t i = 0; i < teammates.size(); i++)
	{
		if (teammates.at(i)->id != 0)
		{
			teammatesNum++;
			if (teammates.at(i)->health != 0)
				teammatesLiveNum++;
		}
	}

	for (size_t i = 0; i < enemy.size(); i++)
	{
		if (enemy.at(i)->id != 0)
		{
			enemyNum++;
			if (enemy.at(i)->health != 0)
				enemyLiveNum++;
		}
	}

	ss << "TM: " << std::setw(2) << enemyLiveNum << "/" << enemyNum;
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 20, Red);
	ss.str("");

	ss << "EN: " << std::setw(2) << teammatesLiveNum << "/" << teammatesNum;
	DrawString(Util::StringManipulation::StringToWstring(ss.str()), x, y + 40, Red);
	ss.str("");
}

void HUD::ShowESP()
{
	for (size_t i = 0; i < 9; i++)
	{
		if (teammates.at(i)->health != 0)
		{
			ShowPlayer(teammates.at(i));
		}
	}

	for (size_t i = 0; i < 10; i++)
	{
		if (enemy.at(i)->health != 0)
		{
			ShowPlayer(enemy.at(i));
		}
	}
}

float t = 0;
void HUD::ShowESPDebug()
{
	localPlayer->team = 2;

	std::unique_ptr<Player> player1 = std::make_unique<Player>();
	player1->bodyScrCoords.x = targetWndWidth / 2 + 200 * sin(t);
	player1->bodyScrCoords.y = targetWndHeight / 2 + 200 * cos(t);
	player1->health = 50 + 50 * sin(t);
	player1->distance = 500 + 100 * sin(t);
	player1->team = 2;
	ShowPlayer(player1);

	std::unique_ptr<Player> player2 = std::make_unique<Player>();
	player2->bodyScrCoords.x = targetWndWidth / 2 - 200 * sin(t);
	player2->bodyScrCoords.y = targetWndHeight / 2 - 200 * cos(t);
	player2->health = 50 + 50 * sin(t);
	player1->team = 3;
	player2->distance = 500 + 100 * sin(t);
	ShowPlayer(player2);
	t += 0.05;
	if (t>= 100)
	{
		t = 0;
	}
}

void HUD::ShowPlayer(const std::unique_ptr<Player> & player)
{
	int x = player->bodyScrCoords.x;
	int y = player->bodyScrCoords.y;
	// int w = 100 * 500 / player->distance;
	// int h = 200 * 500 / player->distance ;
	int w = 50;
	int h = 10;
	if (x * y != 0)
	{
		DirectX::XMFLOAT3 color = player->team == localPlayer->team ? Green : Red;
		//DrawRect(x, targetWndHeight - y, w, h, 1, color);
		DrawLine(x, targetWndHeight - y - h / 2, targetWndWidth / 2, -100, 2, color);
		DrawRect(x, targetWndHeight - y + h / 2 + 5, player->health / 2, 2, 2, color);
		std::stringstream ss;
		ss << std::setfill(' ') << std::setw(2) << player->health << "   " << (int)(player->distance / 10) << "m";
		DrawString(Util::StringManipulation::StringToWstring(ss.str()), x - w, y - h / 2, color);
	}
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		MARGINS margin = { 0, 0, 0, 0 };
		DwmExtendFrameIntoClientArea(hOverlayWnd, &margin);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int HUD::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const LPCWSTR pClassName = L"CSGO Overlay";
	// Register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = RGB(0, 0, 0);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// targetWndName.c_str()
	HWND hTargetWnd = FindWindow(NULL, targetWndName.c_str());
	GetWindowRect(hTargetWnd, &targetRect);
	targetWndWidth = targetRect.right - targetRect.left;
	targetWndHeight = targetRect.bottom - targetRect.top;

	// Create window instance
	hOverlayWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT, pClassName, L"CSGO Hack", WS_POPUP,
		targetRect.left, targetRect.top, targetWndWidth, targetWndHeight,
		nullptr, nullptr, hInstance, nullptr
	);


	// Get the window flags to see if RGB color transparency is supported.
	LONG_PTR ExStyle = GetWindowLongPtr(hOverlayWnd, GWL_EXSTYLE);
	if (ExStyle == 0)
		return 1;

	if ((ExStyle & WS_EX_LAYERED) == 0)
	{
		// Set the window flags to support RGB color transparency.
		if (!SetWindowLongPtr(hOverlayWnd, GWL_EXSTYLE, ExStyle | WS_EX_LAYERED))
			return 2;
	}

	SetWindowPos(hOverlayWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	if (!SetLayeredWindowAttributes(hOverlayWnd, RGB(0, 0, 0), 255, ULW_COLORKEY | LWA_ALPHA)) //LWA_ALPHA
		return 3;

	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	//INT a = UpdateLayeredWindow(hOverlayWnd, GetDC(0), NULL, NULL, NULL, NULL,RGB(0,0,0), &blend,ULW_COLORKEY | LWA_ALPHA);
	UpdateWindow(hOverlayWnd);

	// Show window
	ShowWindow(hOverlayWnd, SW_SHOW);

	// Initializing DirectX
	graphics->Initialize(hOverlayWnd, targetWndWidth, targetWndHeight);

	// Message pump
	MSG msg;

	for (size_t i = 0; i < 9; i++)
	{
		teammates.push_back(std::make_unique<Player>());
	}
	for (size_t i = 0; i < 10; i++)
	{
		enemy.push_back(std::make_unique<Player>());
	}

	while (true)
	{
		if (FunctionEnableFlag::bOverlay)
		{
			ShowWindow(hOverlayWnd, SW_SHOW);

			TimerStart();

			// Update the pos to attach to the game window
			GetWindowRect(hTargetWnd, &targetRect);
			MoveWindow(hOverlayWnd, targetRect.left, targetRect.top, targetWndWidth, targetWndHeight, FALSE);

			// Render pipeline
			graphics->ClearFrame();

			ShowDeclare(10, 5);
			ShowTimeDebug(1520, 500);
			ShowFunctionInfo(1620, 300);
			ShowESPDebug(1620, 700);

			if (FunctionEnableFlag::bESP)
			{
				ShowESPDebug();
			}

			graphics->RenderFrame();

			TimerStop();
		}
		else
		{
			ShowWindow(hOverlayWnd, SW_HIDE);
		}

		while (PeekMessage(&msg, hOverlayWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if (msg.message == WM_NULL)
		//{
		//	exit(0);
		//}

		Sleep(10);
	}
}
