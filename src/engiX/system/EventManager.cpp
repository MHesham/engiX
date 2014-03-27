#include "EventManager.h"

using namespace engiX;

void EventManager::Queue(EventPtr evt)
{
    m_eventQ.push(evt);
}
//----------------------------------------------------------------------------------------------
void EventManager::DeliverMessage(EventPtr evt)
{
    evt
}
//----------------------------------------------------------------------------------------------
void EventManager::Update(const WorldClock& p_clock)
{
    Message* m_message;

    while(!_messageQueue.empty())
    {
        m_message = _messageQueue.front();
        _messageQueue.pop();

        DeliverMessage(m_message);
    }
}
//----------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
    Message* m_message;

    while(!_messageQueue.empty())
    {
        m_message = _messageQueue.front();
        _messageQueue.pop();

        delete m_message;
    }
}
