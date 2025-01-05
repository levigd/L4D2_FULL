#include "CCheat.hpp"
#include "CUtils.hpp"
#include "CGui.hpp"
#include "mh_wrap/mh_wrap.hpp"
#include "CInterfaceWrapper.hpp"
#include "IClientEntityList.hpp"
#include "IClientEntity.hpp"
#include "CClientClass.hpp"
#include "ViewScene.hpp"
#include "IVEngineClient013.hpp"
#include "CInfected.hpp"

CCheat::CCheat(const HINSTANCE hDll) : m_hInstCheat(hDll)
{
	CLogger::Initialize(nullptr, eLogType::ConsoleLog);

	CLogger::Log(eLogLevel::Info, "Trying init cheat.");
}

void CCheat::Process()
{
	//CLogger::Log(eLogLevel::Dev, "%p", CUtils::PatterScan("engine.dll", "74 ? E8 ? ? ? ? FF 15 ? ? ? ? 68"));

	//CLogger::Log(eLogLevel::Info, "%p\n", CUtils::GetModuleBase<PBYTE>("engine.dll"));

	//CLogger::Log(eLogLevel::Dev, "%p", CGui::GetDirectDeviceVTable()[42]);

	EnumWindows(CGui::EnumWindowsProc, 0);
	mh_wrap::instance().add("hkEndScene", CGui::GetDirectDeviceVTable()[42], &CGui::hkEndScene); //Todo: Какой-то прикол с этими девайсами, надо фиксить.
	//mh_wrap::instance().add("hkPresent", CGui::GetDirectDeviceVTable()[15], &CGui::hkPresent);

	//CInterfaceWrapper::InitInterfaceList();
	//int t = 0;
	//CInterfaceWrapper::CreateInterface("IEngineSoundClient003", &t);

	const auto icle = reinterpret_cast<IClientEntityList*>(CInterfaceWrapper::GetInterface("client.dll", "VClientEntityList003"));
	CLogger::Log(eLogLevel::Info, "VClientEntityList003 -> %p <-.", icle);
	
	CLogger::Log(eLogLevel::Dev, "Res -> %i <-.", icle->GetHighestEntityIndexInUsed());

	CLogger::Log(eLogLevel::Dev, "LocalPlayer -> %p <-.", icle->GetClientEntity(1));

	while (!GetAsyncKeyState(VK_DELETE))
	{


		Sleep(1u);
	}

	SetWindowLongPtr(CGui::GetWindow(), GWLP_WNDPROC, (LONG_PTR)CGui::GetOWndProc()); //Todo: Normal unhook.
}

void CCheat::RenderEsp()
{
	static const auto icel = reinterpret_cast<IClientEntityList*>(CInterfaceWrapper::GetInterface("client.dll", "VClientEntityList003"));
	static const auto iec  = reinterpret_cast<IVEngineClient013*>(CInterfaceWrapper::GetInterface("engine.dll", "VEngineClient013")); // Имба нейминг.

	for (int i = 0; i < icel->GetHighestEntityIndexInUsed(); ++i)
	{
		const auto pCurrEntity = icel->GetClientEntity(i);

		if (!pCurrEntity)
			continue;

		const auto szClassName = IClientNetworkable::GetClientClass(pCurrEntity)->GetName();
		//CLogger::Log(eLogLevel::Dev, "%s", szClassName);

		if (strcmp(szClassName, "Infected"))
			continue;

		//CLogger::Log(eLogLevel::Dev, "Infected ptr: %p", pCurrEntity);

		const auto& mat = iec->GetWorldToScreenMatrix();
		//CLogger::Log(eLogLevel::Dev, "Matrix -> %p <-.", mat);
		
		const auto pZombie = reinterpret_cast<CInfected*>(pCurrEntity);
		const Vector zombiePos = Vector(pZombie->position.x, pZombie->position.y, pZombie->position.z);
		const Vector zombieHeadPos = Vector(zombiePos.x, zombiePos.y, zombiePos.z + 65);
		Vector screenPos;//Ноги в 2д
		Vector screenPos2; //голова в 2д

		if (!FrustumTransform(mat, zombiePos, screenPos))
		{
			FrustumTransform(mat, zombieHeadPos, screenPos2);

			float height = screenPos2.y - screenPos.y;
			float width = height / 3;

			const auto pDrawList = ImGui::GetBackgroundDrawList();

			if (config::espSettings::bShowDeathEnemy)
			{
				pDrawList->AddRect(ImVec2(screenPos.x - width, screenPos.y), ImVec2(screenPos.x + width, screenPos.y + height), ImColor(255, 0, 0));
			}
			if (!config::espSettings::bShowDeathEnemy)
			{
				if (pZombie->IsAlive())
					pDrawList->AddRect(ImVec2(screenPos.x - width, screenPos.y), ImVec2(screenPos.x + width, screenPos.y + height), ImColor(255, 0, 0));
			}

			if (config::espSettings::bShowHealth)
			{
				std::string numberStr = std::to_string(pZombie->GetHealth());
				pDrawList->AddText(ImVec2(screenPos.x, screenPos.y), IM_COL32(255, 255, 255, 255), numberStr.c_str());
			}

			if (config::espSettings::bShowSurvivors)
			{
				//TODO:
			}

		}
	}	
}

#pragma warning( disable : 4722 ) // Оно ругается на FreeLibraryAndExitThread(m_hInstCheat, 0x0); потом перепишу.
CCheat::~CCheat()
{
	CLogger::Log(eLogLevel::Info, "Trying shutdown cheat.");

	CLogger::Shutdown();

	FreeLibraryAndExitThread(m_hInstCheat, 0x0);
}
