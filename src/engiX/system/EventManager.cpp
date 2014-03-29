#include "EventManager.h"

using namespace engiX;

void EventManager::Queue(_In_ EventPtr evt)
{
    m_eventQ.push_back(evt);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Update(_In_ const Timer& time)
{
    for (auto evt : m_eventQ)
    {
        EventHandlerListPtr handlers = m_eventListeners[evt->VType()];
        handlers->Fire(evt);
    }

    m_eventQ.clear();
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Unregister(_In_ EventHandlerPtr handler)
{
    for (auto evtEntry : m_eventListeners)
    {
        evtEntry.second->Unregister(handler);
    }
}