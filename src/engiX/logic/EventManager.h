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
        void OnUpdate(_In_ const Timer& time);
        void Queue(_In_ EventPtr evt);
        void Register(_In_ EventHandler* pHandler, _In_ const EventType& type);
        void Unregister(_In_ EventHandler* pHandler, _In_ const EventType& type);
        void Init();
        void Deinit();
        static EventManager* Inst();

    private:
        EventQueue m_eventQ;
        EventRegistry m_eventRegistry;
    };

#define g_EventMgr EventManager::Inst()
}
