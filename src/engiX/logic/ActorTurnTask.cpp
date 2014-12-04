#include "ActorTurnTask.h"
#include "WinGameApp.h"
#include "MathHelper.h"
#include "GameLogic.h"

using namespace engiX;

void ActorTurnTask::OnUpdate(_In_ const Timer& time)
{
    _ASSERTE(g_pApp->Logic()->ActorExist(m_actorId));

    auto& tsfm = g_pApp->Logic()->FindActor(m_actorId).lock()->Get<TransformCmpt>();

    real xRot = tsfm.RotationX() + m_turnVelocities.x * time.DeltaTime();
    tsfm.RotationX(xRot);

    real yRot = tsfm.RotationY() + m_turnVelocities.y * time.DeltaTime();
    tsfm.RotationY(yRot);
}