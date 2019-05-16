#pragma once
#include <Windows.h>
#include <Dwmapi.h>
#include <chrono>
#include <sstream>
#include <iomanip>

#include "..//DirectX/Graphics.h"
#include "..//Game/GameData.h"
#include "..//Game/GameDef.h"
#include "..//Game/GameStruct.h"
#include "..//Util/Logger.h"
#include "..//Util/StepTimer.h"

#include "..//Function/ESP.h"

#pragma comment ( lib, "Dwmapi.lib" )
#pragma comment ( lib, "winmm.lib" )

class HUD
{
public:
	HUD() {}
	HUD(const HUD&) = delete;
	HUD& operator = (const HUD&) = delete;
	~HUD() {}

public:
	void Run() { WinMain(0, 0, 0, 0); }

private:
	const DirectX::XMFLOAT3 Red = { 1,0,0 };
	const DirectX::XMFLOAT3 Green = { 0,1,0 };
	const DirectX::XMFLOAT3 Yellow = { 0.4,1,0 };

private:
	void DrawRect(int x, int y, int w, int h, int stroke, DirectX::XMFLOAT3 color);
	void DrawString(std::wstring str, int x, int y, DirectX::XMFLOAT3 color);
	void DrawLine(int x1, int y1, int x2, int y2, int stroke, DirectX::XMFLOAT3 color);

private:
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
	std::vector<int> timeDelta;
	float smoothDelta = 60;
	int smoothIndex = 60;
	void TimerStart(void);
	void TimerStop();
	float GetFPS();

private:
	void ShowDeclare(int x, int y);
	void ShowTimeDebug(int x, int y);
	void ShowFunctionInfo(int x, int y);
	void ShowESPDebug(int x, int y);

	void ShowESP();
	void ShowPlayer(const std::unique_ptr<Player>& player);
		
private:
	int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

private:
	std::unique_ptr<Graphics> graphics = std::make_unique<Graphics>();
};