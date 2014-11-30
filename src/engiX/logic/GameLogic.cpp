#include "GameLogic.h"
#include <algorithm>
#include "Logger.h"
#include "EventManager.h"
#include "TransformCmpt.h"
#include "ParticlePhysicsCmpt.h"

using namespace engiX;
using namespace std;

GameLogic::~GameLogic()
{
    SAFE_DELETE(m_pView);
}

void GameLogic::OnUpdate(_In_ const Timer& time)
{
    g_EventMgr->OnUpdate(time);

    m_deadActors.clear();
    for (auto actor : m_actors)
    {
        if (actor.second->IsMarkedForRemove())
        {
            m_deadActors.insert(actor.first);
            continue;
        }
        actor.second->OnUpdate(time);
    }

    for (auto deadActor : m_deadActors)
        RemoveActor(deadActor);

    m_taskMgr.OnUpdate(time);

    m_pView->OnUpdate(time);
}

WeakActorPtr GameLogic::FindActor(_In_ ActorID id)
{
    if (id == NullActorID)
        return WeakActorPtr();

    ActorRegistry::iterator itr = m_actors.find(id);

    if (itr != m_actors.end())
        return WeakActorPtr(itr->second);
    else
        return WeakActorPtr();
}

WeakActorPtr GameLogic::FindActor(_In_ const std::wstring &name)
{
    if (name.empty())
        return WeakActorPtr();

    // Perform case sensitive search by actor name in the actor registry
    ActorRegistry::iterator itr = find_if(m_actors.begin(), m_actors.end(), [name](const pair<ActorID, StrongActorPtr>& entry){
        return _wcsicmp(entry.second->Typename(), name.c_str()) == 0;
    });

    if (itr != m_actors.end())
        return WeakActorPtr(itr->second);
    else
        return WeakActorPtr();
}

bool GameLogic::RemoveActor(_In_ ActorID id)
{
    CBRB(m_actors.erase(id) > 0);
    g_EventMgr->Queue(EventPtr(eNEW ActorDestroyedEvt(id, 0)));

    return true;
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
    CBRB(LoadLevel());
    CBRB(m_pView->Init());

    return true;
}
