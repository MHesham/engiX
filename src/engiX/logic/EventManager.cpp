#include "EventManager.h"

using namespace engiX;

EventManager* g_pEventMgrInst = nullptr;


EventManager* EventManager::Inst()
{
    if (g_pEventMgrInst == nullptr)
        g_pEventMgrInst = eNEW EventManager;

    _ASSERTE(g_pEventMgrInst);
    return g_pEventMgrInst;
}

void EventManager::Init() {}

void EventManager::Deinit()
{
    SAFE_DELETE(g_pEventMgrInst);
}

void EventManager::Register(_In_ EventHandlerPtr pHandler, _In_ const EventTypeID& type)
{
    m_eventRegistry[type].Register(pHandler);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Unregister(_In_ EventHandlerPtr pHandler, _In_ const EventTypeID& type)
{
    m_eventRegistry[type].Unregister(pHandler);
}
//////////////////////////////////////////////////////////////////////////
void EventManager::Queue(_In_ EventPtr evt)
{
    m_eventQ.push_back(evt);
    LogInfo("Event %s queued", evt->Typename());
}
//////////////////////////////////////////////////////////////////////////
void EventManager::OnUpdate(_In_ const Timer& time)
{
    for (auto evt : m_eventQ)
    {
        EventHandlerList& handlers = m_eventRegistry[evt->TypeId()];
        handlers.Fire(evt);
        LogInfo("Event %s dispatched", evt->Typename());
    }

    m_eventQ.clear();
}