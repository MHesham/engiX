#pragma once

#include <memory>
#include "engiXDefs.h"
#include "Delegate.h"

namespace engiX
{
    /// <summary>
    /// This generic base class encapsulates the basic functionalities for events
    /// A concrete event class should be inheriting from EventBase and pass to its class template parameter the appropriate delegate class to use
    /// As well as implement Fire(*) and call operator (*) appropriately
    /// </summary>
    template<class TDelegate>
    class EventBase
    {
    public:
        virtual EventBase::~EventBase()
        {
            for(ObserverList::iterator itr = m_observers.begin();
                itr != m_observers.end();
                itr++)
            {
                assert(*itr);
                delete (*itr);
            }

            m_observers.clear();
        }

        /// <summary>Register a delegate to be called when the event is fired</summary>
        /// <param name="pCallback">The delegate to register</param>
        /// <returns>true on successful register, false otherwise</returns>
        ///
        bool operator += (TDelegate* pCallback) { return Register(pCallback); }

        /// <summary>Unregister a delegate</summary>
        /// <param name="pCallback">The delegate to unregister</param>
        /// <returns>true on successful unregister, false otherwise</returns>
        ///
        bool operator -= (const TDelegate* pCallback) { return Unregister(pCallback); }

        /// <summary>Fire the event by calling all registered delegates</summary>

        bool Register(TDelegate* pCallback)
        {
            assert(pCallback);
            if (!pCallback)
                return false;

            for(ObserverList::iterator itr = m_observers.begin();
                itr != m_observers.end();
                itr++)
            {
                assert(*itr);
                if((*itr)->Equals(pCallback))
                    return false;
            }

            m_observers.push_back(pCallback);
            return true;
        }

        bool Unregister(const TDelegate* pCallback)
        {
            assert(pCallback);
            if (!pCallback)
                return false;

            ObserverList::iterator where = m_observers.end();

            for(ObserverList::iterator itr = m_observers.begin();
                itr != m_observers.end();
                itr++)
            {
                assert(*itr);
                if((*itr)->Equals(pCallback))
                {
                    where = itr;
                    break;
                }
            }

            if (where != m_observers.end())
            {
                m_observers.erase(where);
                return true;
            }

            return false;
        }

    protected:
        // Protected constructor to prevent instantiation
        EventBase() {}
        typedef std::list<TDelegate*> ObserverList;
        ObserverList m_observers;
    };

    /// <summary>
    /// Represents an event to which many delegates can be registered to be called when the event is fired
    /// </summary>
    class Event : public EventBase<IDelegate>
    {
    public:
        /// <summary>Fire the event by calling all registered delegates</summary>
        /// <param name="pParam">A parameter to be passed to all delegates when called</param>
        ///
        void operator () () { Fire(); }

        void Event::Fire()
        {
            for(ObserverList::iterator itr = m_observers.begin();
                itr != m_observers.end();
                itr++)
            {
                assert(*itr);
                (*itr)->Call();
            }
        }
    };

    /// <summary>
    /// Represents an event to which many delegates can be registered to be called when the event is fired
    /// </summary>
    template<class TParam>
    class Event1P : public EventBase< IDelegate1P<TParam> >
    {
    public:
        /// <param name="pParam">A parameter to be passed to all delegates when called</param>
        ///
        void operator () (TParam param) { Fire(param); }

        void Fire(TParam param)
        {
            for(ObserverList::iterator itr = m_observers.begin();
                itr != m_observers.end();
                itr++)
            {
                assert(*itr);
                (*itr)->Call(param);
            }
        }
    };
}
