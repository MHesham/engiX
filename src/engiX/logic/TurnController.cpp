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
        tsfm.RotateLocal(0.0f, m_turnVelocity * time.DeltaTime(), 0.0f);
    }

    if (m_isTurningLeft)
    {
        tsfm.RotateLocal(0.0f, -m_turnVelocity * time.DeltaTime(), 0.0f);
    }

    if (m_isTurningUp)
    {
        tsfm.RotateLocal(-m_turnVelocity * time.DeltaTime(), 0.0f, 0.0f);
    }

    if (m_isTurningDown)
    {
        tsfm.RotateLocal(m_turnVelocity * time.DeltaTime(), 0.0f, 0.0f);
    }
}