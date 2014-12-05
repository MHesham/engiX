#include "Actor.h"
#include "Logger.h"
#include "WinGameApp.h"

using namespace engiX;

Actor Actor::Null(L"NullActor");

void Actor::OnUpdate(_In_ const Timer& time)
{
    for (auto& component : m_components)
        component.second->OnUpdate(time);
}

bool Actor::Init()
{
    for (auto& component : m_components)
    {
        if (!component.second->Init())
        {
            LogError("Failed to initialize Component %s[%x] of Actor %s[%x]", component.second->Typename(), component.second->TypeId(), Typename(), Id());
            return false;
        }
    }

    return true;
}
