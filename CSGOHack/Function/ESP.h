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

#include <memory>

#include "../Util/Logger.h"
#include "..//Util/StringManipulation.h"
#include "../Process/Process.h"
#include "../Game/GameData.h"
#include "../Game/GameStruct.h"
#include "../Game/GameDef.h"

void ESP();

// 读取本地玩家信息
void ReadLocalPlayerInfo(void);

// 读取其他玩家信息
void ReadOtherPlayersInfo(void);

// 读取观察矩阵
void ReadViewMatrix(void);

// 绝对空间坐标投影相对屏幕坐标
bool WorldProjectToScreen(Vec3& _world, Vec2& _screen);

// 计算绝对空间距离
float CalculateDistance(Vec3 _vec1, Vec3 _vec2);

// 打印所有玩家信息
void PrintPlayerInfo(void);

// 打印观察矩阵
void PrintViewMatrix(void);
