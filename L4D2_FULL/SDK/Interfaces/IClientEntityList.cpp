#include "IClientEntityList.hpp"
#include "CUtils.hpp"

IClientEntity* IClientEntityList::GetClientEntity(int entnum)
{
	return CUtils::CallVirtual<3, IClientEntity* >(this, entnum);
}

int IClientEntityList::GetHighestEntityIndexInUsed(void)
{
	return CUtils::CallVirtual<6, int>(this);
}
