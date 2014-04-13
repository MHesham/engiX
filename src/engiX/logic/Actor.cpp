#include "Actor.h"
#include "Logger.h"

using namespace engiX;

void Actor::AddComponent(_In_ StrongActorComponentPtr pComponent)
{
    _ASSERTE(m_components.count(pComponent->VId()) == 0);
    _ASSERTE(pComponent->Owner() == NullActorID);

    pComponent->Owner(m_id);
    m_components.insert(make_pair(pComponent->VId(), pComponent));
    LogInfo("%s[%x] has been added to Actor %s[%x]", pComponent->VTypename(), pComponent->VId(), Typename(), Id());
}

void Actor::OnUpdate(_In_ const Timer& time)
{
    for (auto component : m_components)
        component.second->VOnUpdate(time);
}

bool Actor::Init()
{
    for (auto component : m_components)
    {
        if (!component.second->VInit())
        {
            LogError("Failed to initialize Component %s[%x] of Actor %s[%x]", component.second->VTypename(), component.second->VId(), Typename(), Id());
            return false;
        }
    }

    return true;
}
