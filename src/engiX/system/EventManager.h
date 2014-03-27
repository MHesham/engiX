#pragma once

#include <concurrent_queue.h>
#include "Timer.h"
#include "Delegate.h"

namespace engiX
{
    typedef unsigned EventType;

    class Event
    {
    public:
        Event(real timestamp)
            : m_timestamp(m_timestamp) {}

        virtual ~Event() {}
        real Timestamp() const { return m_timestamp; }
        virtual EventType VType() const = 0;

    private:
        EventType m_type;
        real m_timestamp;
    };

    typedef std::shared_ptr<Event> EventPtr;
    typedef concurrency::concurrent_queue<EventPtr> EventQueue;
    typedef IDelegate1P<EventPtr> EventDelegate;

    class EventManager
    {
    public:
        static EventManager& Instance() { static EventManager inst; return inst; }
        void Update(const Timer& time);
        void Queue(EventPtr evt);
        void Register(const EventDelegate& handler, const EventType& type);
        void Unregister(const EventDelegate& handler);

    protected:
        void DeliverMessage(EventPtr evt);

    private:
        EventQueue m_eventQ;

    };

#define g_EventMgr engiX::EventManager::Instance()
}
