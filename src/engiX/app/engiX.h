#pragma once

#include <Windows.h>
#include "Timer.h"
#include "Events.h"
#include "Delegate.h"

namespace engiX
{
    
    class IGameApp
    {

    };

    class IGameView
    {

    };

    class IGameLogic
    {

    };

    typedef IDelegate1P<EventPtr> EventHandler;
    typedef MulticastDelegate1P<EventPtr> EventHandlerList;

    class IEventManager
    {
    public:
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual void Queue(_In_ EventPtr evt) = 0;
        virtual void Register(_In_ EventHandler* pHandler, _In_ const EventType& type) = 0;
        virtual void Unregister(_In_ EventHandler* pHandler, _In_ const EventType& type) = 0;
    };

    class IKeyboardInputManager
    {

    };
}