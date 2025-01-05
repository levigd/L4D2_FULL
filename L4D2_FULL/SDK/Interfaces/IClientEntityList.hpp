#pragma once

//========= From valve sdk 2013. ==============================================//

class IClientEntity;
class ClientClass;
class IClientNetworkable;
class CBaseHandle;
class IClientUnknown;

//-----------------------------------------------------------------------------
// Purpose: Exposes IClientEntity's to engine
//-----------------------------------------------------------------------------
class IClientEntityList
{
public:
	// Get IClientNetworkable interface for specified entity
	virtual IClientNetworkable * GetClientNetworkable(int entnum)					= 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle hEnt)	= 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle hEnt)			= 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	IClientEntity* GetClientEntity(int entnum);
	virtual IClientEntity* GetClientEntityFromHandle(CBaseHandle hEnt)				= 0;

	// Returns number of entities currently in use
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable)		= 0;

	// Returns highest index actually used
	int	GetHighestEntityIndexInUsed(void);

	// Sizes entity list to specified size
	virtual void				SetMaxEntities(int maxents)							= 0;
	virtual int					GetMaxEntities()									= 0;
};

//=============================================================================//