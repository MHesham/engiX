#include "Actor.h"

using namespace engiX;

void Actor::OnUpdate(_In_ const Timer& time)
{
    for (auto component : m_components)
        component.second->VOnUpdate(time);
}