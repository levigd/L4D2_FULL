#include <vector>
#include <string>

#include "CUtils.hpp"

constexpr std::int16_t SIG_UNK_BYTE = -1;

void CUtils::Nop(LPVOID const addr, const size_t size) noexcept
{
	CLogger::Log(eLogLevel::Debug, "Func name -> %s <-. Address -> 0x%p <-. Size -> %i <-.", __FUNCTION__, addr, size);

	constexpr std::uint8_t NOP = 0x90;

	DWORD oldProtect = 0x0;
	VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, NOP, size);
	VirtualProtect(addr, size, oldProtect, &oldProtect);

	//CLogger::Log(eLogLevel::Debug, "Finish setting nop at 0x%p with size -> %i <-. Current line -> %i <-. Current file name -> %s <-.", addr, size, __LINE__, __FILE__);
}

static inline void FromStrToBytes(const char* szSignature, std::vector<std::int16_t>& output)
{
	for (char* pCurrSimv = const_cast<char*>(szSignature); pCurrSimv < szSignature + strlen(szSignature); ++pCurrSimv)
	{
		if (*pCurrSimv == '?')
		{
			++pCurrSimv;
			output.push_back(SIG_UNK_BYTE);
		}
		else
		{
			output.push_back(static_cast<std::int16_t>(std::strtoul(pCurrSimv, &pCurrSimv, 16)));
		}
	}
}

LPVOID CUtils::PatterScan(const char* szModuleName, const char* szSignature) noexcept
{
	LPVOID res = nullptr;

	if (!szModuleName || *szModuleName == '\0' || !szSignature || *szSignature == '\0')
	{
		CLogger::Log(eLogLevel::Error, "%s failed because of invalid params.", __FUNCTION__);

		return nullptr;
	}

	const auto moduleBase = CUtils::GetModuleBase<PBYTE>(szModuleName);
	if (!moduleBase)
	{
		CLogger::Log(eLogLevel::Error, "Failed to get module base for patter scan. Module -> %s <-. Signature -> %s <-.", szModuleName, szSignature);
		return nullptr;
	}

	std::vector<std::int16_t> bytes;
	FromStrToBytes(szSignature, bytes);

	/*
	CLogger::Log(eLogLevel::Dev, "bytes.size = %i.", bytes.size());
	for (auto& i : bytes)
	{
		CLogger::Log(eLogLevel::Dev, "Byte -> %x <-", i);
	}
	*/

	MODULEINFO mi = {};
	GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(moduleBase), &mi, sizeof(MODULEINFO));

	for (PBYTE currAddr = moduleBase; currAddr < moduleBase + mi.SizeOfImage; ++currAddr)
	{
		bool bFound = true;

		for (size_t i = 0; i < bytes.size(); ++i)
		{
			if (bytes.at(i) != SIG_UNK_BYTE && currAddr[i] != bytes.at(i))
			{
				bFound = false;
				break;
			}
		}

		if (bFound)
		{
			res = currAddr;
			break;
		}
	}

	CLogger::Log(eLogLevel::Debug, "Patter scaner found signature -> %s <- at address -> %p <-.", szSignature, res);

	return res;
}

