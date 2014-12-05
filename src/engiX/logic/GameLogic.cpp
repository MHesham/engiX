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
    for (auto& actor : m_actors)
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

Actor& GameLogic::GetActor(_In_ ActorID id)
{
    auto it = m_actors.find(id);

    if (it != m_actors.end())
        return *it->second;
    else
        return Actor::Null;
}

Actor& GameLogic::GetActor(_In_ const wchar_t* pName)
{
    for (auto& p : m_actors)
    {
        if (_wcsicmp(p.second->Typename(), pName) == 0)
            return *p.second;
    }

    return Actor::Null;
}

bool GameLogic::RemoveActor(_In_ ActorID id)
{
    CBRB(m_actors.erase(id) > 0);
    g_EventMgr->Queue(EventPtr(eNEW ActorDestroyedEvt(id, 0)));

    return true;
}

bool GameLogic::AddInitActor(_In_ ActorUniquePtr pActor) 
{ 
    if (!pActor->Init())
    {
        LogError("Actor %s[%d] initialization failed", pActor->Typename(), pActor->Id());
        m_actors.erase(pActor->Id());
        return false;
    }

    g_EventMgr->Queue(EventPtr(eNEW ActorCreatedEvt(pActor->Id(), 0.0f)));

    m_actors[pActor->Id()] = std::move(pActor);

    return true;
}


bool GameLogic::Init()
{
    CBRB(LoadLevel());
    CBRB(m_pView->Init());

    return true;
}
