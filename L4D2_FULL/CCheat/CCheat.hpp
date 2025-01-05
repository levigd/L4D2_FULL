#pragma once

#include <Windows.h>

class CCheat
{
public:
	CCheat() = default;
	CCheat(const HINSTANCE hDll);

	void Process();
	static void RenderEsp();

	~CCheat();

private:
	HINSTANCE m_hInstCheat;
};

namespace config
{
	inline bool bEnableAim	= false;
	inline bool bEnableEsp	= false;
	inline bool bEnableBhop = false;

	namespace espSettings
	{
		inline bool bShowHealth		= false;
		inline bool bShowDeathEnemy = false;
		inline bool bShowSurvivors	= false;
	}
}