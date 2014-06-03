#include "TurnController.h"
#include "EventManager.h"

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
    if (m_actorTsfm.expired())
        return;

    shared_ptr<TransformCmpt> pTsfm = m_actorTsfm.lock();

    if (m_isTurningRight)
    {
        pTsfm->RotationY(pTsfm->RotationY() + (m_turnVelocity * time.DeltaTime()));
    }

    if (m_isTurningLeft)
    {
        pTsfm->RotationY(pTsfm->RotationY() - (m_turnVelocity * time.DeltaTime()));
    }

    if (m_isTurningUp)
    {
        pTsfm->RotationX(pTsfm->RotationX() - (m_turnVelocity * time.DeltaTime()));
    }

    if (m_isTurningDown)
    {
        pTsfm->RotationX(pTsfm->RotationX() + (m_turnVelocity * time.DeltaTime()));
    }
}