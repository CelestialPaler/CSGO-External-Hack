#include "Manager.h"

void Manager::Run(void)
{
	Initialize();

	std::thread generalThread(GeneralThreadFunc);
	std::thread triggerBotThread(TriggerBotThreadFunc);
	//std::thread overlayThread(OverlayThreadFunc);

	while (true)
	{
		HandleKeyBoardInput();
		HandleMouseInput();
		UpdateFromConfigFile();

		Sleep(10);
	}
}

void Manager::Initialize(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	Process* proc = Process::GetInstance();
	proc->Attach(targetProcName);

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "Manager Initialized.";
		msg.szResult = "OK";
		logger->Write(msg);
	}
}

void Manager::GeneralThreadFunc(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "General Thread Now Runing.";
		logger->Write(msg);
	}

	while (true)
	{
		if (FunctionEnableFlag::bOverlay)
		{
			Process* proc = Process::GetInstance();
			QWORD clientAddr = proc->moduleBaseAddr[L"client_panorama.dll"];

			for (size_t i = 1; i < 64; i++)
			{
				DWORD targetPlayerAddr = proc->ReadDWORD(clientAddr + (QWORD)hazedumper::signatures::dwEntityList + (QWORD)(i * 0x10));
				if (targetPlayerAddr != NULL)
				{
					BOOL targetPlayerIsSpotted = proc->ReadBool(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted);
					if (targetPlayerIsSpotted == FALSE)
						do
						{
							proc->WriteBool(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted, TRUE);
						} while (!proc->ReadBool(targetPlayerAddr + (QWORD)hazedumper::netvars::m_bSpotted));
				}
			}
		}
		Sleep(10);
	}
}

void Manager::OverlayThreadFunc(void)
{
	HUD csgoHUD;
	csgoHUD.Run();
}

void Manager::TriggerBotThreadFunc(void)
{	
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	// 日志
	{
		Util::Logger::LogDisc msg;
		msg.emPriority = Util::Logger::Priority::INFO;
		msg.szFrom = __func__;
		msg.szMsg = "TiggerBot Thread Now Runing.";
		logger->Write(msg);
	}

	while (true)
	{
		if (FunctionEnableFlag::bTriggerBot)
		{
			TriggerBot();
		}
		Sleep(1);
	}
}

void Manager::HandleKeyBoardInput(void)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	if (GetAsyncKeyState(VK_F1) & 1)
	{
		Toggle(FunctionEnableFlag::bOverlay);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F1 - Overlay";
			msg.szResult = FunctionEnableFlag::bOverlay ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
	if (GetAsyncKeyState(VK_F2) & 1)
	{
		Toggle(FunctionEnableFlag::bTriggerBot);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F2 - TriggerBot";
			msg.szResult = FunctionEnableFlag::bTriggerBot ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
	if (GetAsyncKeyState(VK_F4) & 1)
	{
		Toggle(FunctionEnableFlag::bESP);

		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::INFO;
			msg.szFrom = __func__;
			msg.szMsg = "F4 - ESP";
			msg.szResult = FunctionEnableFlag::bESP ? "Enabled." : "Disabled.";
			logger->Write(msg);
		}
	}
}
