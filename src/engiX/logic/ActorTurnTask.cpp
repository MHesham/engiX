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

    tsfm.RotateLocal(
        m_turnVelocities.x * time.DeltaTime(),
        m_turnVelocities.y * time.DeltaTime(),
        m_turnVelocities.z * time.DeltaTime());
}