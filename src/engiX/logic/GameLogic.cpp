#include "GameLogic.h"
#include "Logger.h"
#include "EventManager.h"

using namespace engiX;

void GameLogic::VOnUpdate(_In_ const Timer& time)
{
    g_EventMgr->OnUpdate(time);

    for (auto actor : m_actors)
        actor.second->OnUpdate(time);
}

WeakActorPtr GameLogic::GetActor(ActorID id)
{
    ActorRegistry::iterator itr = m_actors.find(id);

    if (itr != m_actors.end())
        return WeakActorPtr(itr->second);
    else
        return WeakActorPtr();
}

void GameLogic::VInit()
{

}

void GameLogic::Deinit()
{

}