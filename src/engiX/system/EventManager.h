#pragma once

#include <list>
#include <memory>
#include <map>
#include "Timer.h"
#include "Delegate.h"
#include "Events.h"

namespace engiX
{
    typedef IDelegate1P<EventPtr> EventHandler;
    typedef MulticastDelegate1P<EventPtr> EventHandlerList;
    typedef std::list<EventPtr> EventQueue;
    typedef std::map<EventType, EventHandlerList> EventRegistry;

    class EventManager
    {
    public:
        static EventManager& Instance() { static EventManager inst; return inst; }
        void Update(_In_ const Timer& time);
        void Queue(_In_ EventPtr evt);
        void Register(_In_ EventHandler* pHandler, _In_ const EventType& type);
        void Unregister(_In_ EventHandler* pHandler, _In_ const EventType& type);

    private:
        EventQueue m_eventQ;
        EventRegistry m_eventRegistry;
    };

#define g_EventMgr engiX::EventManager::Instance()
}
