#include "HealthReader.h"

void HealthReader(void)
{
	Process* proc = Process::GetInstance();
	DWORD dwPlayerBase = proc->ReadInt32(proc->moduleBaseAddr[L"client_panorama.dll"] + hazedumper::signatures::dwLocalPlayer);
	playerHealth = proc->ReadInt32(dwPlayerBase + hazedumper::netvars::m_iHealth);

	// È¡µÃLogger
	Util::Logger* logger = Util::Logger::GetInstance();
	Util::Logger::LogDisc msg;
	msg.emPriority = Util::Logger::Priority::INFO;
	msg.szFrom = __func__;
	msg.szMsg = "Reading Player Health.";
	msg.szTarget = Util::StringManipulation::WstringToString(targetProcName);
	msg.szResult = std::to_string(playerHealth);
	logger->Write(msg);
}
