#include "ActorTurnTask.h"
#include "WinGameApp.h"
#include "MathHelper.h"

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

        real xRot = pTsfm->RotationX() + m_turnVelocities.x * time.DeltaTime();
        pTsfm->RotationX(xRot);

        real yRot = pTsfm->RotationY() + m_turnVelocities.y * time.DeltaTime();
        pTsfm->RotationY(yRot);
    }
}