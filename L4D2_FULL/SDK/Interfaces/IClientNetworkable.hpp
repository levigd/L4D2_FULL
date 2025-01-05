#pragma once

#include <Windows.h> //For PBYTE

#include "CLogger.hpp"

class ClientClass;
class IClientEntity;

//========= From valve sdk 2013. ==============================================//

class IClientNetworkable
{
public:
	// Supplied automatically by the IMPLEMENT_CLIENTCLASS macros.
	ClientClass* GetClientClass();

	//Not valve
	// Что за ужас?
	//TODO:
	[[nodiscard]] static ClientClass* GetClientClass(IClientEntity* pEnt)
	{
		if (!pEnt)
		{
			CLogger::Log(eLogLevel::Error, "%s. pEnt = nullptr.", __FUNCTION__);

			return nullptr;
		}

		typedef ClientClass* (__thiscall* t_GetClientClass)(IClientEntity* this_);

		DWORD* addr = (DWORD*)(reinterpret_cast<PBYTE>(pEnt) + 0x8);
		DWORD* vTable = *(DWORD**)(addr);
		DWORD* GetClientClassInTable = (DWORD*)((char*)vTable + 0x4); //Optimize in 1 line

		t_GetClientClass pGetClientClass = (t_GetClientClass)(*(DWORD**)GetClientClassInTable);
		//CLogger::Log(eLogLevel::Dev, "Normal -> %p <-.", pGetClientClass);

		
		return pGetClientClass(pEnt);
	}
};

//=============================================================================//