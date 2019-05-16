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

#include "ESP.h"

float viewMatrix[4][4] = { 0 };

void ESP()
{
	ReadLocalPlayerInfo();
	ReadViewMatrix();
	ReadOtherPlayersInfo();
}

void ReadLocalPlayerInfo(void)
{
	Process* proc = Process::GetInstance();
	QWORD clientBaseAddr = proc->moduleBaseAddr[L"client_panorama.dll"];
	QWORD engineBaseAddr = proc->moduleBaseAddr[L"engine.dll"];

	DWORD localPlayerAddr = proc->ReadDWORD(clientBaseAddr + (QWORD)hazedumper::signatures::dwLocalPlayer);

	localPlayer->health = proc->ReadInt32(localPlayerAddr + (QWORD)hazedumper::netvars::m_iHealth);
	localPlayer->team = proc->ReadInt32(localPlayerAddr + (QWORD)hazedumper::netvars::m_iTeamNum);
	localPlayer->id = proc->ReadInt32(localPlayerAddr + (QWORD)hazedumper::netvars::m_iAccountID);
	localPlayer->bodyGameCoords.x = proc->ReadFloat(localPlayerAddr + (QWORD)hazedumper::netvars::m_vecOrigin + sizeof(float) * 0);
	localPlayer->bodyGameCoords.y = proc->ReadFloat(localPlayerAddr + (QWORD)hazedumper::netvars::m_vecOrigin + sizeof(float) * 1);
	localPlayer->bodyGameCoords.z = proc->ReadFloat(localPlayerAddr + (QWORD)hazedumper::netvars::m_vecOrigin + sizeof(float) * 2);
}

void ReadOtherPlayersInfo(void)
{
	Process* proc = Process::GetInstance();
	QWORD clientBaseAddr = proc->moduleBaseAddr[L"client_panorama.dll"];

	// 表示下一个需要写入数据的玩家
	int teammateIndex = 0, enemyIndex = 0;

	// 遍历所有玩家
	/// 从1开始，排除本地玩家
	for (size_t i = 1; i < 40; i++)
	{
		DWORD otherPlayerAddr = proc->ReadDWORD(clientBaseAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)i * 0x10);

		// 若玩家不存在
		if (otherPlayerAddr == 0) break;

		// 创建临时玩家
		std::unique_ptr<Player> tempPlayer = std::make_unique<Player>();

		// 读取基本信息
		tempPlayer->health = proc->ReadInt32(otherPlayerAddr + (QWORD)hazedumper::netvars::m_iHealth);
		tempPlayer->team = proc->ReadInt32(otherPlayerAddr + (QWORD)hazedumper::netvars::m_iTeamNum);
		tempPlayer->id = proc->ReadInt32(otherPlayerAddr + (QWORD)hazedumper::netvars::m_iAccountID);
		tempPlayer->isSpotted = proc->ReadBool(otherPlayerAddr + (QWORD)hazedumper::netvars::m_bIsScoped) & (1 << localPlayer->id - 1);

		// 读取身体绝对空间坐标
		tempPlayer->bodyGameCoords.x = proc->ReadFloat(otherPlayerAddr + (QWORD)hazedumper::netvars::m_vecOrigin + sizeof(float) * 0);
		tempPlayer->bodyGameCoords.y = proc->ReadFloat(otherPlayerAddr + (QWORD)hazedumper::netvars::m_vecOrigin + sizeof(float) * 1);
		tempPlayer->bodyGameCoords.z = proc->ReadFloat(otherPlayerAddr + (QWORD)hazedumper::netvars::m_vecOrigin + sizeof(float) * 2);

		// 计算身体相对屏幕坐标
		if (WorldProjectToScreen(tempPlayer->bodyGameCoords, tempPlayer->bodyScrCoords))
		{
			tempPlayer->bodyScrCoords.x -= targetRect.left;
			tempPlayer->bodyScrCoords.y -= targetRect.top;
		}
		else
		{
			// 如果投影失败，就归零处理
			tempPlayer->bodyScrCoords.x = 0;
			tempPlayer->bodyScrCoords.y = 0;
		}

		// 计算与玩家的距离
		tempPlayer->distance = CalculateDistance(tempPlayer->bodyGameCoords, localPlayer->bodyGameCoords);

		// 判断是否是队友
		if (tempPlayer->team != 0 && tempPlayer->team == localPlayer->team)
			teammates.at(teammateIndex++).swap(tempPlayer);
		else
			enemy.at(enemyIndex++).swap(tempPlayer);
	}
}

void ReadViewMatrix(void)
{
	Process* proc = Process::GetInstance();
	QWORD clientBaseAddr = proc->moduleBaseAddr[L"client_panorama.dll"];

	BYTE* temp = proc->ReadBytes(clientBaseAddr + (QWORD)hazedumper::signatures::dwViewMatrix, 4 * 4 * 4);
	std::memcpy(viewMatrix, temp, sizeof(BYTE) * 4 * 4 * 4);
}

bool WorldProjectToScreen(Vec3& _world, Vec2& _screen)
{
	float w = 0.0f;
	_screen.x = viewMatrix[0][0] * _world.x + viewMatrix[0][1] * _world.y + viewMatrix[0][2] * _world.z + viewMatrix[0][3];
	_screen.y = viewMatrix[1][0] * _world.x + viewMatrix[1][1] * _world.y + viewMatrix[1][2] * _world.z + viewMatrix[1][3];
	w = viewMatrix[3][0] * _world.x + viewMatrix[3][1] * _world.y + viewMatrix[3][2] * _world.z + viewMatrix[3][3];
	if (w < 0.01f)
		return false;
	float invw = 1.0f / w;
	_screen.x = invw * _screen.x;
	_screen.y = invw * _screen.y;
	int width = (int)targetWndWidth;
	int height = (int)(targetWndHeight);
	float x = width / 2;
	float y = height / 2;
	x += 0.5 * _screen.x * width + 0.5;
	y -= 0.5 * _screen.y * height + 0.5;
	_screen.x = x + targetRect.left;
	_screen.y = y + targetRect.top;
	return true;
}

float CalculateDistance(Vec3 _vec1, Vec3 _vec2)
{
	return sqrt(pow(_vec1.x - _vec2.x, 2) + pow(_vec1.y - _vec2.y, 2) + pow(_vec1.z - _vec2.z, 2));
}

void PrintPlayerInfo(void)
{
	std::cout << "\nLocalPlayer" << std::endl;
	std::cout
		<< localPlayer->team << " | "
		<< std::setw(3) << std::setfill(' ') << std::setprecision(4)
		<< localPlayer->health << " | ("
		<< localPlayer->bodyGameCoords.x << ","
		<< localPlayer->bodyGameCoords.y << ","
		<< localPlayer->bodyGameCoords.z << ")"
		<< std::endl;
	std::cout << "\nTeammatePlayer" << std::endl;
	for (size_t i = 0; i < teammates.size(); i++)
	{
		std::cout
			<< teammates.at(i)->team << " | "
			<< std::setw(3) << std::setfill(' ') << std::setprecision(4)
			<< teammates.at(i)->health << " | ("
			<< teammates.at(i)->bodyGameCoords.x << ","
			<< teammates.at(i)->bodyGameCoords.y << ","
			<< teammates.at(i)->bodyGameCoords.z << ") | ("
			<< teammates.at(i)->bodyScrCoords.x << ","
			<< teammates.at(i)->bodyScrCoords.y << ") |"
			<< teammates.at(i)->distance << " | "
			<< teammates.at(i)->isSpotted
			<< std::endl;
	}
	std::cout << "\nEnemyPlayer" << std::endl;
	for (size_t i = 0; i < enemy.size(); i++)
	{
		std::cout
			<< enemy.at(i)->team << " | "
			<< std::setw(3) << std::setfill(' ') << std::setprecision(4)
			<< enemy.at(i)->health << " | ("
			<< enemy.at(i)->bodyGameCoords.x << ","
			<< enemy.at(i)->bodyGameCoords.y << ","
			<< enemy.at(i)->bodyGameCoords.z << ") | ("
			<< enemy.at(i)->bodyScrCoords.x << ","
			<< enemy.at(i)->bodyScrCoords.y << ") |"
			<< enemy.at(i)->distance << " | "
			<< enemy.at(i)->isSpotted
			<< std::endl;
	}
}

void PrintViewMatrix(void)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			std::cout << viewMatrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}
}
