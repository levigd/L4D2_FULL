#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include <TlHelp32.h>
#include <psapi.h>

#include "CLogger.hpp"

class CUtils
{
public:
	//TODO:
	/*
	template<typename T>
	static void Read(const T* const addr, const T& res) noexcept
	{
		CLogger::Log(eLogLevel::Debug, "Start read memory at 0x%p.", addr);

		DWORD oldProtect = 0x0;
		VirtualProtect(addr, sizeof(T*), PAGE_EXECUTE_READWRITE, &oldProtect);
		res = *addr;
		VirtualProtect(addr, sizeof(T*), oldProtect, &oldProtect);

		CLogger::Log(eLogLevel::Debug, "Finish read memory at 0x%p with res -> %x <-", addr, res);

	}
	*/
	template<typename T>
	static T Read(const T* const addr) noexcept
	{
		T res = {};
		CLogger::Log(eLogLevel::Debug, "Start read memory at 0x%p.", addr);

		DWORD oldProtect = 0x0;
		VirtualProtect(addr, sizeof(T*), PAGE_EXECUTE_READWRITE, &oldProtect);
		res = *addr;
		VirtualProtect(addr, sizeof(T*), oldProtect, &oldProtect);

		CLogger::Log(eLogLevel::Debug, "Finish read memory at 0x%p with res -> %x <-", addr, res);

	}

	template<typename T>
	static void Write(T* const addr, const T value) noexcept
	{
		CLogger::Log(eLogLevel::Debug, "Start writing memory at 0x%p.", addr);

		DWORD oldProtect = 0x0;
		VirtualProtect(addr, sizeof(T*), PAGE_EXECUTE_READWRITE, &oldProtect);
		*addr = value;
		VirtualProtect(addr, sizeof(T*), oldProtect, &oldProtect);

		CLogger::Log(eLogLevel::Debug, "Finish writing memory at 0x%p with value -> %x <-", addr, value);
	}

	static void Nop(LPVOID const addr, const size_t size) noexcept;

	//Пора уже выучить шаблоны, чтобы не копировать код...
	template <int index, typename Type, typename... Arguments>
	static Type CallVirtual(void* instance, Arguments... arguments)
	{
		typedef Type(__thiscall* tFunction)(void* instance, Arguments... arguments);
		tFunction func = (*(tFunction**)instance)[index];
		return func(instance, arguments...);
	}

public:
	// Pattern scan in module. Signature type: 74 ? E8 ? ? ? ? FF 15 ? ? ? ? 68
	[[nodiscard]] static LPVOID PatterScan(const char* szModuleName, const char* szSignature) noexcept;

public:
	template<typename T>
	[[nodiscard]] static T GetModuleBase(const char* szModuleName) noexcept
	{
		CLogger::Log(eLogLevel::Info, "Trying get %s imagebase", szModuleName);

		T res = reinterpret_cast<T>(0);
		auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	
		MODULEENTRY32 me = {};
		me.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hSnap, &me))
		{
			CLogger::Log(eLogLevel::Error, "!Module32First. GetLastError -> %i <-", GetLastError());
			return res;
		}

		do
		{
			if (!strcmp(me.szModule, szModuleName))
			{
				res = reinterpret_cast<T>(me.modBaseAddr);
			}
		} while (Module32Next(hSnap, &me));

		return res;
	}
};