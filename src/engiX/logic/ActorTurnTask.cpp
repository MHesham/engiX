#include "ActorTurnTask.h"
#include "WinGameApp.h"

using namespace engiX;

bool ActorTurnTask::Init()
{
    CBRB(Task::Init());

    WeakActorPtr actor = g_pApp->Logic()->FindActor(m_actorId);
    CBRB(!actor.expired());

    m_actorTsfm = actor.lock()->Get<TransformCmpt>();
    CBRB(!actor.expired());

    return true;
}

void ActorTurnTask::OnUpdate(_In_ const Timer& time)
{
    if (!m_actorTsfm.expired())
    {
        auto pTsfm = m_actorTsfm.lock();

        pTsfm->RotationX(pTsfm->RotationX() + m_turnVelocities.x * time.DeltaTime());
        pTsfm->RotationY(pTsfm->RotationY() + m_turnVelocities.y * time.DeltaTime());
    }
}