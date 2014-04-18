#pragma once

#include <memory>
#include "engiXDefs.h"
#include "Vector.h"
#include "Actor.h"

namespace engiX
{
    typedef unsigned EventTypeID;

    class Event
    {
    public:
        Event(real timestamp)
            : m_timestamp(timestamp) {}

        virtual ~Event() {}
        real Timestamp() const { return m_timestamp; }
        virtual EventTypeID TypeId() const = 0;
        virtual const wchar_t* Typename() const = 0;

    private:
        real m_timestamp;
    };

    typedef std::shared_ptr<Event> EventPtr;

    class ToggleCameraEvt : public Event
    {
    public:
        static const EventTypeID TypeID = 0x7D030697;

        ToggleCameraEvt(real timestamp) : Event(timestamp) {}
        EventTypeID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"ToggleCameraEvt"; }
    };

    class DisplaySettingsChangedEvt : public Event
    {
    public:
        static const EventTypeID TypeID = 0xDC31296F;

        DisplaySettingsChangedEvt(real timestamp) : Event(timestamp) {}
        EventTypeID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"DisplaySettingsChangedEvt"; }
    };

    class ActorCreatedEvt : public Event
    {
    public:
        static const EventTypeID TypeID = 0x275AF762;

        ActorCreatedEvt(ActorID actorId, real timestamp) :
            Event(timestamp),
            m_actorId(actorId) {}

        EventTypeID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"ActorCreatedEvt"; }
        ActorID ActorId() const { return m_actorId; }
        
    protected:
        ActorID m_actorId;
    };

    class ActorDestroyedEvt : public Event
    {
    public:
        static const EventTypeID TypeID = 0x697EC9B1;

        ActorDestroyedEvt(real timestamp) : Event(timestamp) {}
        EventTypeID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"ActorDestroyedEvt"; }
    };
}