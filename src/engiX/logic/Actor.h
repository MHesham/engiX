#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Timer.h"

namespace engiX
{
    class Actor;
    class ActorComponent;

    typedef unsigned ComponentID;
    typedef unsigned ActorID;
    typedef std::wstring ActorTypename;

    typedef std::shared_ptr<Actor> StrongActorPtr;
    typedef std::weak_ptr<Actor> WeakActorPtr;
    typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;
    typedef std::weak_ptr<ActorComponent> WeakActorComponentPtr;

    const ActorID NullActorID = 0;
    const ComponentID NullComponentID = 0;

    class ActorComponent
    {
    public:
        ActorComponent() : m_owner(NullActorID) {}
        virtual ~ActorComponent() {}
        virtual ComponentID TypeId() const = 0;
        virtual const wchar_t* Typename() const = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual bool Init() = 0;
        virtual ActorID Owner() const { return m_owner; } 
        virtual void Owner(ActorID owner) 
        {
            // We assume that that actor owner is set once in the component lifetime
            // Later, the owner setting will be done by some XML Actor loading routine
            _ASSERTE(m_owner == NullActorID); m_owner = owner; 
        }

    protected:
        ActorID m_owner;
    };

    class Actor
    {
    public:
        typedef std::unordered_map<ComponentID, StrongActorComponentPtr> ActorComponentRegistry;

        Actor(const wchar_t* actorTypename) :
            m_id(++m_lastActorId),
            m_typename(actorTypename)
        { }

        ActorID Id() const { return m_id; }
        const wchar_t* Typename() const { return m_typename.c_str(); }
        void OnUpdate(_In_ const Timer& time);
        bool Init();

        // template function for retrieving components
        template <class ComponentType>
        std::weak_ptr<ComponentType> GetComponent(_In_ ComponentID id)
        {
            ActorComponentRegistry::iterator findIt = m_components.find(id);
            if (findIt != m_components.end())
            {
                StrongActorComponentPtr pBase(findIt->second);
                std::shared_ptr<ComponentType> pSub(std::static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
                std::weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
                return pWeakSub;  // return the weak pointer
            }
            else
            {
                return std::weak_ptr<ComponentType>();
            }
        }

        const ActorComponentRegistry& GetComponents() const { return m_components; }
        void AddComponent(_In_ StrongActorComponentPtr pComponent);

    private:
        ActorID m_id;
        std::wstring m_typename;
        ActorComponentRegistry m_components;

        static ActorID m_lastActorId;
    };
}