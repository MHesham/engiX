#pragma once

#include "Timer.h"
#include "Actor.h"
#include "ViewInterfaces.h"

namespace engiX
{
    class GameLogic
    {
    public:
        typedef std::unordered_map<ActorID, StrongActorPtr> ActorRegistry;

        GameLogic() : m_pView(nullptr) {}
        virtual ~GameLogic();
        virtual void OnUpdate(_In_ const Timer& time);
        virtual bool Init();

        void View(_In_ IGameView* pView) { m_pView = pView; }
        IGameView* View() { return m_pView; }
        WeakActorPtr FindActor(_In_ ActorID id);

    protected:
        virtual void LoadLevel() = 0;
        bool AddInitActor(_In_ StrongActorPtr pActor);

    private:
        ActorRegistry m_actors;
        IGameView* m_pView;
    };
}