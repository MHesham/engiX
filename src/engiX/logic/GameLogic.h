#pragma once

#include "Timer.h"
#include "Actor.h"

namespace engiX
{
    class GameLogic
    {
    public:
        typedef std::unordered_map<ActorID, StrongActorPtr> ActorRegistry;

        virtual ~GameLogic() {}
        virtual void VOnUpdate(_In_ const Timer& time);
        virtual void VInit();
        virtual void Deinit();

    protected:
        virtual void VLoadLevel() = 0;
        void AddActor(StrongActorPtr pActor) { m_actors.insert(std::make_pair(pActor->Id(), pActor)); }
        WeakActorPtr GetActor(ActorID id);
        
    private:
        ActorRegistry m_actors;
    };
}