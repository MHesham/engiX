#pragma once

#include <memory>
#include "engiXDefs.h"
#include "Vector.h"

namespace engiX
{
    enum EventType
    {
        EVT_KeyDown,
        EVT_KeyUp,
        EVT_MouseDown,
        EVT_MouseUp,
        EVT_MouseMove
    };

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

    typedef char KeyCode;

    class KeyDownEvent : public Event
    {
    public:
        KeyDownEvent(real timestamp, KeyCode code)
            : Event(timestamp),
            m_code(code)
        {}

        EventType VType() const { return EVT_KeyDown; }
        KeyCode Code() const { return m_code; }

    private:
        KeyCode m_code;
    };

    class KeyUpEvent : public Event
    {
    public:
        KeyUpEvent(real timestamp, KeyCode code)
            : Event(timestamp),
            m_code(code)
        {}

        EventType VType() const { return EVT_KeyUp; }
        KeyCode Code() const { return m_code; }

    private:
        KeyCode m_code;
    };

    typedef char MouseButtonCode;

    class MouseUpEvent : public Event
    {
    public:
        MouseUpEvent(real timestamp, Point3 pos, MouseButtonCode code)
            : Event(timestamp),
            m_pos(pos),
            m_code(code)
        {}

        EventType VType() const { return EVT_MouseUp; }
        const Point3& Position() const { return m_pos; }
        MouseButtonCode Code() const { return m_code; }

    private:
        Point3 m_pos;
        MouseButtonCode m_code;
    };

    class MouseDownEvent : public Event
    {
    public:
        MouseDownEvent(real timestamp, Point3 pos, MouseButtonCode code)
            : Event(timestamp),
            m_pos(pos),
            m_code(code)
        {}

        EventType VType() const { return EVT_MouseDown; }
        const Point3& Position() const { return m_pos; }
        MouseButtonCode Code() const { return m_code; }

    private:
        Point3 m_pos;
        MouseButtonCode m_code;
    };

    class MouseMoveEvent : public Event
    {
    public:
        MouseMoveEvent(real timestamp, Point3 pos, MouseButtonCode code)
            : Event(timestamp),
            m_pos(pos)
        {}

        EventType VType() const { return EVT_MouseMove; }
        const Point3& Position() const { return m_pos; }

    private:
        Point3 m_pos;
    };

}