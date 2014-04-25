#include "GameLogic.h"
#include "Logger.h"
#include "EventManager.h"

using namespace engiX;

GameLogic::~GameLogic()
{
    SAFE_DELETE(m_pView);
}

void GameLogic::OnUpdate(_In_ const Timer& time)
{
    g_EventMgr->OnUpdate(time);

    for (auto actor : m_actors)
        actor.second->OnUpdate(time);

    m_pView->OnUpdate(time);
}

WeakActorPtr GameLogic::FindActor(_In_ ActorID id)
{
    ActorRegistry::iterator itr = m_actors.find(id);

    if (itr != m_actors.end())
        return WeakActorPtr(itr->second);
    else
        return WeakActorPtr();
}

bool GameLogic::AddActor(_In_ StrongActorPtr pActor) 
{ 
    CBRB(m_actors.insert(std::make_pair(pActor->Id(), pActor)).second);
    g_EventMgr->Queue(EventPtr(eNEW ActorCreatedEvt(pActor->Id(), 0.0f)));

    return true;
}


bool GameLogic::Init()
{
    LoadLevel();

    for (auto actor : m_actors)
    {
        if (!actor.second->Init())
        {
            LogError("Actor %s[%d] initialization failed", actor.second->Typename(), actor.second->Id());
            return false;
        }
    }

    CBRB(m_pView->Init());

    return true;
}