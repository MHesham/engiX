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
            : m_timestamp(m_timestamp) {}

        virtual ~Event() {}
        real Timestamp() const { return m_timestamp; }
        virtual EventType VType() const = 0;
        virtual const wchar_t* VToString() const = 0;

    private:
        EventType m_type;
        real m_timestamp;
    };

    typedef std::shared_ptr<Event> EventPtr;
}