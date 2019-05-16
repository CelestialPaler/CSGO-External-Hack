#include "TriggerBot.h"

void TriggerBot(void)
{
	Process* proc = Process::GetInstance();
	QWORD clientAddr = proc->moduleBaseAddr[L"client_panorama.dll"];
	DWORD localPlayerAddr = proc->ReadUint32(clientAddr + hazedumper::signatures::dwLocalPlayer);

	unsigned int aimID = proc->ReadUint32(localPlayerAddr + hazedumper::netvars::m_iCrosshairId);
	unsigned int localTeam = proc->ReadUint32(localPlayerAddr + hazedumper::netvars::m_iTeamNum);

	if (aimID >= 1 && aimID <= 64)
	{
		DWORD targetPlayerAimAddr = proc->ReadUint32(clientAddr + hazedumper::signatures::dwEntityList + (QWORD)((aimID - 1) * 0x10));
		unsigned int aimTeam = proc->ReadUint32(targetPlayerAimAddr + hazedumper::netvars::m_iTeamNum);

		if (localTeam != aimTeam)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
			Sleep(10);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
		}
	}
}
