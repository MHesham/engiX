#pragma once

#include <list>
#include <memory>
#include <map>
#include "engiX.h"

namespace engiX
{
    typedef std::list<EventPtr> EventQueue;
    typedef std::map<EventTypeID, EventHandlerList> EventRegistry;

    class EventManager : public IEventManager
    {
    public:
        static EventManager& Instance() { static EventManager inst; return inst; }
        void OnUpdate(_In_ const Timer& time);
        void Queue(_In_ EventPtr evt);
        void Register(_In_ EventHandlerPtr pHandler, _In_ const EventTypeID& type);
        void Unregister(_In_ EventHandlerPtr pHandler, _In_ const EventTypeID& type);
        void Init();
        void Deinit();
        static EventManager* Inst();

    private:
        EventQueue m_eventQ;
        EventRegistry m_eventRegistry;
    };

#define g_EventMgr EventManager::Inst()

#define REGISTER_EVT(CALLEE, EVT) \
    g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &##CALLEE##::##On##EVT), EVT##::TypeID);

}
