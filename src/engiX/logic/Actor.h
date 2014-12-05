#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "Timer.h"
#include "engiXDefs.h"
#include "Object.h"

#define DECLARE_COMPONENT(CmptName, CmptGuid) \
    static const ComponentID TypeID = CmptGuid; \
    ComponentID TypeId() const { return TypeID; } \
    const wchar_t* Typename() const { return L#CmptName; } \

namespace engiX
{
    class Actor;
    class ActorComponent;

    typedef unsigned ComponentID;
    typedef unsigned ActorID;
    typedef unsigned ActorTypeID;
    typedef std::wstring ActorName;
    typedef std::wstring ActorTypename;

    typedef Actor* ActorPtr;
    typedef const Actor* ConstActorPtr;
    typedef std::unique_ptr<Actor> ActorUniquePtr;
    typedef std::shared_ptr<Actor> StrongActorPtr;
    typedef std::weak_ptr<Actor> WeakActorPtr;

    typedef ActorComponent* ActorComponentPtr;
    typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;
    typedef std::weak_ptr<ActorComponent> WeakActorComponentPtr;

    const ActorID NullActorID = 0;
    const ComponentID NullComponentID = 0;

    class ActorComponent : public Object
    {
    public:
        DISALLOW_COPY_AND_ASSIGN(ActorComponent);

        ActorComponent() : m_pOwner(nullptr) {}
        virtual ~ActorComponent() {}
        virtual ComponentID TypeId() const = 0;
        virtual void OnUpdate(_In_ const Timer& time) = 0;
        virtual bool Init() { return true; }
        ActorPtr Owner() const { return m_pOwner; }
        void Owner(ActorPtr pOwner) { m_pOwner = pOwner; }

    protected:
        ActorPtr m_pOwner;
    };

    class Actor : public Object
    {
    public:
        DISALLOW_COPY_AND_ASSIGN(Actor);

        typedef std::unordered_map<ComponentID, std::unique_ptr<ActorComponent>> ActorComponentRegistry;

        Actor(const wchar_t* actorTypename) :
            m_id(++sm_lastActorId),
            m_typename(actorTypename),
            m_markedForRemove(false)
        { }

        ActorID Id() const { return m_id; }
        const wchar_t* Typename() const { return m_typename.c_str(); }
        void OnUpdate(_In_ const Timer& time);
        bool Init();

        template<class ComponentType>
        bool HasA() const { return m_components.count(ComponentType::TypeID) > 0; }

        template <class ComponentType>
        ComponentType& Get()
        {
            ActorComponentRegistry::iterator findIt = m_components.find(ComponentType::TypeID);
            // You can't get a component that does not exist
            // If you are unsure, use Actor::HasA<ComponenetType> before attempting
            // to access a certain component
            _ASSERTE(findIt != m_components.end());
            return *(ComponentType*)findIt->second.get();;
        }

        const ActorComponentRegistry& GetComponents() const { return m_components; }

        template<class T, class... Args>
        T& Add(const Args&... args)
        {
            auto pCmpt = eNEW T(args...);

            // An actor should have only 0 or 1 of each component
            _ASSERTE(m_components.count(pCmpt->TypeId()) == 0);

            pCmpt->Owner(this);
            m_components.insert(make_pair(pCmpt->TypeId(), std::unique_ptr<T>(pCmpt)));
            LogVerbose("%s[%x] has been added to Actor %s[%x]", pCmpt->Typename(), pCmpt->TypeId(), Typename(), Id());

            return *pCmpt;
        }

        bool IsMarkedForRemove() const { return m_markedForRemove; }
        void MarkForRemove() { m_markedForRemove = true; }
        bool IsNull() const { return m_components.empty(); }

        static Actor Null;

    private:

        ActorID m_id;
        std::wstring m_typename;
        ActorComponentRegistry m_components;
        bool m_markedForRemove;

        static ActorID sm_lastActorId;
    };
}