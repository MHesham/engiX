#include "Actor.h"
#include "Logger.h"

using namespace engiX;

void Actor::AddComponent(_In_ StrongActorComponentPtr pComponent)
{
    _ASSERTE(m_components.count(pComponent->TypeId()) == 0);
    _ASSERTE(pComponent->Owner() == NullActorID);

    pComponent->Owner(m_id);
    m_components.insert(make_pair(pComponent->TypeId(), pComponent));
    LogInfo("%s[%x] has been added to Actor %s[%x]", pComponent->Typename(), pComponent->TypeId(), Typename(), Id());
}

void Actor::OnUpdate(_In_ const Timer& time)
{
    for (auto component : m_components)
        component.second->OnUpdate(time);
}

bool Actor::Init()
{
    for (auto component : m_components)
    {
        if (!component.second->Init())
        {
            LogError("Failed to initialize Component %s[%x] of Actor %s[%x]", component.second->Typename(), component.second->TypeId(), Typename(), Id());
            return false;
        }
    }

    return true;
}
