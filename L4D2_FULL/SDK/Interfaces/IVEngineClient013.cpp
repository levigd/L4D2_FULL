#include "IVEngineClient013.hpp"
#include "CUtils.hpp"
#include "ViewScene.hpp"

void IVEngineClient013::GetScreenSize(int& width, int& height)
{
	CUtils::CallVirtual<5, void, int&, int&>(this, width, height);
}

const VMatrix& IVEngineClient013::GetWorldToScreenMatrix()
{
	return CUtils::CallVirtual<37, VMatrix&>(this);
}
