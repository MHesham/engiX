#pragma once

#include <memory>
#include "engiXDefs.h"
#include "Vector.h"

namespace engiX
{
    typedef unsigned EventType;

    class Event
    {
    public:
        Event(real timestamp)
            : m_timestamp(timestamp) {}

        virtual ~Event() {}
        real Timestamp() const { return m_timestamp; }
        virtual EventType VType() const = 0;
        virtual const wchar_t* VToString() const = 0;

    private:
        real m_timestamp;
    };

    typedef std::shared_ptr<Event> EventPtr;

    class ToggleCameraEvt : public Event
    {
    public:
        static const EventType TypeID = 0x7D030697;

        ToggleCameraEvt(real timestamp) : Event(timestamp) {}
        EventType VType() const { return TypeID; }
        const wchar_t* VToString() const { return L"ToggleCameraEvt"; }
    };
}