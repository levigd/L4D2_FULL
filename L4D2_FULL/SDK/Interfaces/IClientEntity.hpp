#pragma once

#include "IClientUnknown.hpp"
#include "IClientRenderable.hpp"
#include "IClientNetworkable.hpp"
#include "IClientThinkable.hpp"
#include "ViewScene.hpp"

struct Ray_t;
struct QAngle;
class CGameTrace;
typedef CGameTrace trace_t;
class CMouthInfo;
class IClientEntityInternal;
struct SpatializationInfo_t;


//-----------------------------------------------------------------------------
// Purpose: All client entities must implement this interface.
//-----------------------------------------------------------------------------
class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	// Delete yourself.
	virtual void			Release(void) = 0;

	// Network origin + angles
	virtual const Vector& GetAbsOrigin(void) const = 0;
	virtual const QAngle& GetAbsAngles(void) const = 0;

	virtual CMouthInfo* GetMouth(void) = 0;

	// Retrieve sound spatialization info for the specified sound on this entity
	// Return false to indicate sound is not audible
	virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;
};
