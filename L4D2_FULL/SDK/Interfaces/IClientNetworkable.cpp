#include "IClientNetworkable.hpp"
#include "CClientClass.hpp"
#include "CUtils.hpp"

ClientClass* IClientNetworkable::GetClientClass()
{
    return CUtils::CallVirtual<5, ClientClass*>(this);
}
