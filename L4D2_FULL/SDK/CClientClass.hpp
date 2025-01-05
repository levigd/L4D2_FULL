#pragma once

#include "IClientNetworkable.hpp"

//========= From valve sdk 2013. ==============================================//

class ClientClass;
class IClientNetworkable;

// The serial number that gets passed in is used for ehandles.
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

//-----------------------------------------------------------------------------
// Purpose: Client side class definition
//-----------------------------------------------------------------------------
class ClientClass
{
public:
	const char* GetName() const noexcept
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;	// Only called for event objects.
	const char* m_pNetworkName;
	void* m_pRecvTable;						//RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int						m_ClassID;	// Managed by the engine.
};

//=============================================================================//