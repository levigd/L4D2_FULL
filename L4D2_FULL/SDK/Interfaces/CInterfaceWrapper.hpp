#pragma once

#include "CUtils.hpp"

//========= From valve sdk 2013. ==============================================//

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

// Used internally to register classes.
class InterfaceReg
{
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char* pName);

public:
	InstantiateInterfaceFn	m_CreateFn;
	const char* m_pName;

	InterfaceReg* m_pNext; // For the global list.
	static InterfaceReg* s_pInterfaceRegs;
};

// interface return status
enum
{
	IFACE_OK = 0,
	IFACE_FAILED
};

//=============================================================================//

class CInterfaceWrapper
{
public:
	//=============================================================================//
	//Заглушка пока не будет реализована std::map с интерфейсами всеми.
	[[nodiscard]] static void* GetInterface(const char* szModuleName, const char* szInterfaceName); 
	//=============================================================================//

	static void InitInterfaceList()
	{
		InterfaceReg::s_pInterfaceRegs = **reinterpret_cast<InterfaceReg***>(reinterpret_cast<PBYTE>(CUtils::PatterScan("engine.dll", "8B 35 ? ? ? ? 57 85 F6 74 ? 8B 7D ? 8B 46 ? 8B CF")) + 0x2);
	}
	[[nodiscard]] static void* CreateInterface(const char* szInterfaceName, int* pReturnCode) noexcept;
};