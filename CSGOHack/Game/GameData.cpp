#include "GameData.h"

// ��ǰ�����汾
const std::wstring devVersion = L"0.6b";

// Ŀ�����ID
DWORD targetPID;
// Ŀ���������
const std::wstring targetProcName = L"csgo.exe"; //notepad.exe 
// Ŀ�괰������
const std::wstring targetWndName = L"Counter-Strike: Global Offensive";//target.txt - Notepad

// Ŀ�괰�ھ��
HWND hTargetWnd;
HWND hOverlayWnd;
// Ŀ�괰��Rect
RECT targetRect;
// Ŀ�괰�ڿ��
unsigned int targetWndWidth;
// Ŀ�괰�ڳ���
unsigned int targetWndHeight;

// ������ң���ʦ��а��
std::unique_ptr<Player> localPlayer = std::make_unique<Player>();
// ���еĶ���
std::vector<std::unique_ptr<Player>> teammates;
// ���еĵ���
std::vector<std::unique_ptr<Player>> enemy;

namespace FunctionEnableFlag
{
	bool bOverlay = false;
	bool bTriggerBot = false;
	bool bAimBot = false;
	bool bESP = false;
	bool bRadarHack = false;
	bool bNULL = false;
}

namespace ThreadExistFlag
{
	bool bGeneral = false;
	bool bOverlay = false;
	bool bTriggerBot = false;
	bool bAimBot = false;
	bool bESP = false;
	bool bRadarHack = false;
	bool bNULL = false;
}
