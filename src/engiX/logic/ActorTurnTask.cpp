#include "ActorTurnTask.h"
#include "WinGameApp.h"
#include "MathHelper.h"
#include "GameLogic.h"

using namespace engiX;

void ActorTurnTask::OnUpdate(_In_ const Timer& time)
{
    auto& a = g_pApp->Logic()->GetActor(m_actorId);
    _ASSERTE(!a.IsNull());
    auto& tsfm = a.Get<TransformCmpt>();

    real xRot = tsfm.RotationX() + m_turnVelocities.x * time.DeltaTime();
    tsfm.RotationX(xRot);

    real yRot = tsfm.RotationY() + m_turnVelocities.y * time.DeltaTime();
    tsfm.RotationY(yRot);
}