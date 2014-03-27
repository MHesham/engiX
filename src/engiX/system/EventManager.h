#pragma once

#include <list>
#include <memory>
#include <map>
#include "Timer.h"
#include "Delegate.h"

namespace engiX
{
    typedef unsigned MessageType;

    class Message
    {
    public:
        Message(real timestamp)
            : m_timestamp(m_timestamp) {}

        virtual ~Message() {}
        real Timestamp() const { return m_timestamp; }
        virtual MessageType VType() const = 0;

    private:
        MessageType m_type;
        real m_timestamp;
    };

    typedef std::shared_ptr<Message> MessagePtr;
    typedef IDelegate1P<MessagePtr> EventHandler;
    typedef std::shared_ptr<EventHandler> EventDelegatePtr;
    typedef std::list<MessagePtr> EventQueue;
    typedef std::list<EventHandler> EventListenerList;
    typedef std::map<MessageType, EventListenerList> EventListenerMap;

    class EventManager
    {
    public:
        static EventManager& Instance() { static EventManager inst; return inst; }
        void Update(_In_ const Timer& time);
        void Queue(_In_ MessagePtr evt);
        void Register(_In_ const EventHandler& handler, _In_ const MessageType& type);
        void Unregister(_In_ const EventHandler& handler);

    private:
        EventQueue m_eventQ;
        EventListenerMap m_eventListeners;
    };

#define g_EventMgr engiX::EventManager::Instance()
}
