#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Timer.h"
#include "engiXDefs.h"

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
        ActorComponent() : m_pOwner(nullptr) {}
        virtual ~ActorComponent() {}
        virtual ComponentID TypeId() const = 0;
        virtual const wchar_t* Typename() const = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual bool Init() = 0;
        Actor* Owner() const { return m_pOwner; } 
        void Owner(Actor* pOwner) { m_pOwner = pOwner; } 

    protected:

        Actor *m_pOwner;
    };

    class Actor
    {
    public:
        typedef std::unordered_map<ComponentID, StrongActorComponentPtr> ActorComponentRegistry;

        Actor(const wchar_t* actorTypename) :
            m_id(++m_lastActorId),
            m_typename(actorTypename),
            m_markedForRemove(false)
        { }

        ActorID Id() const { return m_id; }
        const wchar_t* Typename() const { return m_typename.c_str(); }
        void OnUpdate(_In_ const Timer& time);
        bool Init();

        // template function for retrieving components
        template <class ComponentType>
        std::weak_ptr<ComponentType> Get()
        {
            ActorComponentRegistry::iterator findIt = m_components.find(ComponentType::TypeID);
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
        void Add(_In_ StrongActorComponentPtr pComponent);

        template<class T, class... Args>
        std::shared_ptr<T> Add(const Args&... args)
        {
            std::shared_ptr<T> pCmpt(eNEW T(args...));
            Add(pCmpt);
            return pCmpt;
        }

        bool IsMarkedForRemove() const { return m_markedForRemove; }
        void MarkForRemove() { m_markedForRemove = true; }

    private:
        ActorID m_id;
        std::wstring m_typename;
        ActorComponentRegistry m_components;
        bool m_markedForRemove;

        static ActorID m_lastActorId;
    };
}