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
        virtual ~GameLogic() {}
        virtual void VOnUpdate(_In_ const Timer& time);
        virtual bool VInit();
        virtual void Deinit();

        void View(std::shared_ptr<IGameView> pView) { m_pView = pView; }
        std::shared_ptr<IGameView> View() { return m_pView; }
        WeakActorPtr GetActor(ActorID id);

    protected:
        virtual void VLoadLevel() = 0;
        void AddActor(StrongActorPtr pActor) { m_actors.insert(std::make_pair(pActor->Id(), pActor)); }

    private:
        ActorRegistry m_actors;
        std::shared_ptr<IGameView> m_pView;
    };
}