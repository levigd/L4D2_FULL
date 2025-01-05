#include <string.h>

#include "CInterfaceWrapper.hpp"
#include "CLogger.hpp"

//typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

void* CInterfaceWrapper::GetInterface(const char* szModuleName, const char* szInterfaceName)
{
	constexpr const char* szCreateInterfaceFuncName = "CreateInterface";

	if (!szModuleName || !szInterfaceName || *szModuleName == '\0' || *szInterfaceName == '\0')
	{
		CLogger::Log(eLogLevel::Error, "%s. Bad params.", __FUNCTION__);

		return nullptr;
	}

	const auto hModule = CUtils::GetModuleBase<HMODULE>(szModuleName);
	if (!hModule)
	{
		CLogger::Log(eLogLevel::Error, "%s. Bad hModule value %x.", __FUNCTION__, hModule);

		return nullptr;
	}

	const auto pCreateInterface = GetProcAddress(hModule, szCreateInterfaceFuncName);
	if (!pCreateInterface)
	{
		CLogger::Log(eLogLevel::Error, "%s. Failed to get ptr to CreateInterface func. GetLastError -> %i <-.", __FUNCTION__, GetLastError());
		
		return nullptr;
	}

	CreateInterfaceFn CreateInterface = reinterpret_cast<CreateInterfaceFn>(pCreateInterface);

	return CreateInterface(szInterfaceName, nullptr); //2 param - optional
}

void* CInterfaceWrapper::CreateInterface(const char* szInterfaceName, int* pReturnCode) noexcept
{
	InterfaceReg* pCur;

	for (pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext)
	{
		CLogger::Log(eLogLevel::Dev, "Interface name -> %s <-.", pCur->m_pName);
		if (strcmp(pCur->m_pName, szInterfaceName) == 0)
		{
			if (pReturnCode)
			{
				*pReturnCode = IFACE_OK;
			}
			return pCur->m_CreateFn();
		}
	}

	if (pReturnCode)
	{
		*pReturnCode = IFACE_FAILED;
	}

    return nullptr;
}

InterfaceReg* InterfaceReg::s_pInterfaceRegs = nullptr;

InterfaceReg::InterfaceReg(InstantiateInterfaceFn fn, const char* pName) : m_pName(pName)
{
	m_CreateFn			= fn;
	m_pNext				= s_pInterfaceRegs;
	s_pInterfaceRegs	= this;
}
