#include "EventManager.h"

using namespace engiX;

void EventManager::Queue(_In_ EventPtr evt)
{
    m_eventQ.push_back(evt);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Update(_In_ const Timer& time)
{
    for each(auto evt in m_eventQ)
    {
        
    }
}

