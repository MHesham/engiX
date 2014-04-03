#pragma once

#include <list>
#include <memory>
#include <map>
#include "engiX.h"

namespace engiX
{
    typedef std::list<EventPtr> EventQueue;
    typedef std::map<EventType, EventHandlerList> EventRegistry;

    class EventManager : public IEventManager
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
}
