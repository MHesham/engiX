#include "TurnController.h"
#include "EventManager.h"

using namespace engiX;
using namespace std;

bool TurnController::Init()
{
    g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &TurnController::OnStartTurnRight), StartTurnRightEvt::TypeID);
    g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &TurnController::OnEndTurnRight), EndTurnRightEvt::TypeID);
    g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &TurnController::OnStartTurnLeft), StartTurnLeftEvt::TypeID);
    g_EventMgr->Register(MakeDelegateP1<EventPtr>(this, &TurnController::OnEndTurnLeft), EndTurnLeftEvt::TypeID);

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
}