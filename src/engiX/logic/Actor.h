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

    class ActorComponent
    {
    public:
        ~ActorComponent() {}
        ComponentID Id() const { return m_id; }
        virtual const wchar_t* VTypename() const = 0;
        virtual void VOnUpdate(_In_ const Timer& time) = 0;

    private:
        ComponentID m_id;
        static ActorID m_lastActorId;
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

        // template function for retrieving components
        template <class ComponentType>
        std::weak_ptr<ComponentType> GetComponent(ComponentID id)
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
        void AddComponent(StrongActorComponentPtr pComponent) { m_components.insert(make_pair(pComponent->Id(), pComponent)); }

    private:
        ActorID m_id;
        std::wstring m_typename;
        ActorComponentRegistry m_components;

        static ActorID m_lastActorId;
    };
}