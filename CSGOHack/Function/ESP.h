#pragma once 

#include <memory>

#include "../Util/Logger.h"
#include "..//Util/StringManipulation.h"
#include "../Process/Process.h"
#include "../Game/GameData.h"
#include "../Game/GameStruct.h"
#include "../Game/GameDef.h"

void ESP();

// ��ȡ���������Ϣ
void ReadLocalPlayerInfo(void);

// ��ȡ���������Ϣ
void ReadOtherPlayersInfo(void);

// ��ȡ�۲����
void ReadViewMatrix(void);

// ���Կռ�����ͶӰ�����Ļ����
bool WorldProjectToScreen(Vec3& _world, Vec2& _screen);

// ������Կռ����
float CalculateDistance(Vec3 _vec1, Vec3 _vec2);

// ��ӡ���������Ϣ
void PrintPlayerInfo(void);

// ��ӡ�۲����
void PrintViewMatrix(void);
