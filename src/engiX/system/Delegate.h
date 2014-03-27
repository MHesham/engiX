//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#pragma once

#include <list>
#include <cassert>
#include <process.h>
#include <windows.h>

namespace engiX
{
    // Used to cast Win32 CreateThread threastart to CRT _beginthreadex threadstart
    typedef unsigned(__stdcall *PfnCrtThreadStartEx)(void*);

    /// <summary>
    /// Represents a delegate to a member function that returns void and takes no parameters
    /// </summary>
    class IDelegate
    {
    public:
        virtual ~IDelegate() {}
        virtual bool Equals(const IDelegate* pOther) const = 0;
        virtual void operator()() = 0;
        virtual void Call() = 0;
    };

    /// <summary>
    /// Represents a delegate to a member function that returns void and takes 1 parameter which is TParam
    /// </summary>
    template<class TParam>
    class IDelegate1P
    {
    public:
        virtual ~IDelegate1P() {}
        virtual bool Equals(const IDelegate1P* pOther) const = 0;
        virtual void operator()(TParam param) = 0;
        virtual void Call(TParam param) = 0;
    };

    /// <summary>
    /// Represents a 0 parameter delegate to a method inside TReciever template class
    /// </summary>
    template<class TReciever>
    class Delegate : public IDelegate
    {
    public:
        typedef void (TReciever::*Callback)();

        Delegate(TReciever* pObj, Callback pfnCallback) :
            m_pObj(pObj), m_pfnCallback(pfnCallback) {}

        bool Equals(const IDelegate* pOther) const
        {
            const Delegate<TReciever>* other = static_cast<const Delegate<TReciever>*>(pOther);
            assert(pOther);
            return other->m_pObj == m_pObj && other->m_pfnCallback == m_pfnCallback;
        }

        void operator()()
        {
            assert(m_pObj);
            assert(m_pfnCallback);
            (m_pObj->*m_pfnCallback)();
        }

        void Call()
        {
            assert(m_pObj);
            assert(m_pfnCallback);
            (m_pObj->*m_pfnCallback)();
        }

        void CallAsync()
        {
            HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, (PfnCrtThreadStartEx)CallAsyncThreadStart_Static, this, 0, nullptr);
            (void)CloseHandle(hThread);
        }

        static DWORD WINAPI CallAsyncThreadStart_Static(_In_  LPVOID lpParameter)
        {
            IDelegate* pDelegate = reinterpret_cast<IDelegate*>(lpParameter);

            if (pDelegate)
            {
                pDelegate->Call();
            }

            return 0;
        }

    private:
        Callback    m_pfnCallback;
        TReciever   *m_pObj;
    };

    /// <summary>
    /// Represents a 1 parameter delegate to a method inside TReciever template class
    /// </summary>
    template<class TReciever, class TParam>
    class Delegate1P : public IDelegate1P<TParam>
    {
    public:
        typedef void (TReciever::*Callback)(TParam param);

        Delegate1P(TReciever* pObj, Callback pfnCallback) :
            m_pObj(pObj), m_pfnCallback(pfnCallback) {}

        bool Equals(const IDelegate1P* pOther) const
        {
            const Delegate1P<TReciever, TParam>* other = static_cast<const Delegate1P<TReciever, TParam>*>(pOther);
            assert(pOther);
            return other->m_pObj == m_pObj && other->m_pfnCallback == m_pfnCallback;
        }

        virtual void operator()(TParam param)
        {
            assert(m_pObj);
            assert(m_pfnCallback);
            (m_pObj->*m_pfnCallback)(param);
        }

        virtual void Call(TParam param)
        {
            assert(m_pObj);
            assert(m_pfnCallback);
            (m_pObj->*m_pfnCallback)(param);
        }

    private:
        Callback    m_pfnCallback;
        TReciever   *m_pObj;
    };

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