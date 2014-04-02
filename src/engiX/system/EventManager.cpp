#include "EventManager.h"

using namespace engiX;

void EventManager::Register(_In_ EventHandler* pHandler, _In_ const EventType& type)
{
    m_eventRegistry[type].Register(pHandler);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Unregister(_In_ EventHandler* pHandler, _In_ const EventType& type)
{
    m_eventRegistry[type].Unregister(pHandler);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Queue(_In_ EventPtr evt)
{
    m_eventQ.push_back(evt);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Update(_In_ const Timer& time)
{
    for (auto evt : m_eventQ)
    {
        EventHandlerList& handlers = m_eventRegistry[evt->VType()];
        handlers.Fire(evt);
    }

    m_eventQ.clear();
}