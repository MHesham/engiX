#pragma once

#include <set>
#include "Timer.h"
#include "Actor.h"
#include "ViewInterfaces.h"
#include "CollisionDetection.h"
#include "TaskManager.h"
#include "ParticleForceGen.h"

namespace engiX
{
    class GameLogic
    {
    public:
        typedef std::unordered_map<ActorID, ActorUniquePtr> ActorRegistry;

        GameLogic() : m_pView(nullptr) {}
        virtual ~GameLogic();
        virtual void OnUpdate(_In_ const Timer& time);
        virtual bool Init();

        void View(_In_ IGameView* pView) { m_pView = pView; }
        IGameView* View() { return m_pView; }
        Actor& GetActor(_In_ ActorID id);
        Actor& GetActor(_In_ const wchar_t* pName);
        ParticleForceRegistry& ForceRegistry() { return m_forceRegistry; }

    protected:
        virtual bool LoadLevel() = 0;
        bool AddInitActor(_In_ ActorUniquePtr pActor);
        bool RemoveActor(_In_ ActorID);

        TaskManager m_taskMgr;

    private:
        ActorRegistry m_actors;
        IGameView* m_pView;
        std::set<ActorID> m_deadActors;
        ParticleForceRegistry m_forceRegistry;
    };
}