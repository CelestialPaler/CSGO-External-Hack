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
