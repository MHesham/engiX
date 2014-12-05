#include "TurnController.h"
#include "EventManager.h"
#include "GameApp.h"
#include "GameLogic.h"

using namespace engiX;
using namespace std;

bool TurnController::Init()
{
    REGISTER_EVT(TurnController, StartTurnRightEvt);
    REGISTER_EVT(TurnController, EndTurnRightEvt);
    REGISTER_EVT(TurnController, StartTurnLeftEvt);
    REGISTER_EVT(TurnController, EndTurnLeftEvt);
    REGISTER_EVT(TurnController, StartForwardThrustEvt);
    REGISTER_EVT(TurnController, EndForwardThrustEvt);
    REGISTER_EVT(TurnController, StartBackwardThrustEvt);
    REGISTER_EVT(TurnController, EndBackwardThrustEvt);

    return true;
}

void TurnController::Update(_In_ const Timer& time)
{
    auto& a = g_pApp->Logic()->GetActor(m_actorId);

    if (a.IsNull())
        return;

    auto& tsfm = a.Get<TransformCmpt>();

    if (m_isTurningRight)
    {
        tsfm.RotationY(tsfm.RotationY() + (m_turnVelocity * time.DeltaTime()));
    }

    if (m_isTurningLeft)
    {
        tsfm.RotationY(tsfm.RotationY() - (m_turnVelocity * time.DeltaTime()));
    }

    if (m_isTurningUp)
    {
        tsfm.RotationX(tsfm.RotationX() - (m_turnVelocity * time.DeltaTime()));
    }

    if (m_isTurningDown)
    {
        tsfm.RotationX(tsfm.RotationX() + (m_turnVelocity * time.DeltaTime()));
    }
}