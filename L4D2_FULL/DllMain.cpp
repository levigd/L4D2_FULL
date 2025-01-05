#include <cstdio>

#include "DllMain.hpp"

static void MainThread(HINSTANCE hInstDll)
{
	//CLogger::Initialize(nullptr, eLogType::ConsoleLog);

	//CLogger::Log(eLogLevel::Dev, "%p", CUtils::PatterScan("engine.dll", "74 ? E8 ? ? ? ? FF 15 ? ? ? ? 68"));

	//CUtils::Nop((LPVOID)(0x6A1D11D5), 5);

	//CLogger::Log(eLogLevel::Info, "%p\n", CUtils::GetModuleBase<PBYTE>("engine.dll"));

	CCheat cheat(hInstDll);

	cheat.Process();
}

DWORD WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hInstDll);

		const auto hThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hInstDll, 0, nullptr);
		if (hThread)
			CloseHandle(hThread);
		break;
	}

	default:
		break;
	}
	return TRUE;
}