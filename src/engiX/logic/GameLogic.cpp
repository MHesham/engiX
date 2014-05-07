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

bool GameLogic::AddInitActor(_In_ StrongActorPtr pActor) 
{ 
    CBRB(m_actors.insert(std::make_pair(pActor->Id(), pActor)).second);

    if (!pActor->Init())
    {
        LogError("Actor %s[%d] initialization failed", pActor->Typename(), pActor->Id());
        m_actors.erase(pActor->Id());
        return false;
    }
    
    g_EventMgr->Queue(EventPtr(eNEW ActorCreatedEvt(pActor->Id(), 0.0f)));

    return true;
}


bool GameLogic::Init()
{
    LoadLevel();

    CBRB(m_pView->Init());

    return true;
}