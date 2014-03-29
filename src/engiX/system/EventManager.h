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
    typedef std::shared_ptr<EventHandler> EventHandlerPtr;

    typedef MulticastDelegate1P<EventPtr> EventHandlerList;
    typedef std::shared_ptr<EventHandlerList> EventHandlerListPtr;

    typedef std::list<EventPtr> EventQueue;
    typedef std::map<EventType, EventHandlerListPtr> EventListenerMap;

    class EventManager
    {
    public:
        static EventManager& Instance() { static EventManager inst; return inst; }
        void Update(_In_ const Timer& time);
        void Queue(_In_ EventPtr evt);
        void Register(_In_ EventHandlerPtr handler, _In_ const EventType& type) { m_eventListeners[type]->Register(handler); }
        void Unregister(_In_ EventHandlerPtr handler, _In_ const EventType& type) { m_eventListeners[type]->Unregister(handler); }
        void Unregister(_In_ EventHandlerPtr handler);

    private:
        EventQueue m_eventQ;
        EventListenerMap m_eventListeners;
    };

#define g_EventMgr engiX::EventManager::Instance()
}
