#pragma once

#include <memory>
#include "engiXDefs.h"
#include "Timer.h"
#include "TransformCmpt.h"
#include "Events.h"

namespace engiX
{
    class TurnController
    {
    public:
        TurnController::TurnController() :
            m_isTurningRight(false),
            m_isTurningLeft(false),
            m_isTurningUp(false),
            m_isTurningDown(false),
            m_turnVelocity(1.5f)
        { }

        bool Init();
        void Control(_In_ ActorID id) { m_actorId = id; }
        void Update(_In_ const Timer& time);

    protected:
        void OnStartTurnRightEvt(EventPtr evt)
        {
            _ASSERTE(!m_isTurningRight);
            m_isTurningRight = true;
        }

        void OnEndTurnRightEvt(EventPtr evt)
        {
            _ASSERTE(m_isTurningRight);
            m_isTurningRight = false;
        }

        void OnStartTurnLeftEvt(EventPtr evt)
        {
            _ASSERTE(!m_isTurningLeft);
            m_isTurningLeft = true;
        }

        void OnEndTurnLeftEvt(EventPtr evt)
        {
            _ASSERTE(m_isTurningLeft);
            m_isTurningLeft = false;
        }

        void OnStartForwardThrustEvt(EventPtr evt)
        {
            _ASSERTE(!m_isTurningUp);
            m_isTurningUp = true;
        }

        void OnEndForwardThrustEvt(EventPtr evt)
        {
            _ASSERTE(m_isTurningUp);
            m_isTurningUp = false;
        }

        void OnStartBackwardThrustEvt(EventPtr evt)
        {
            _ASSERTE(!m_isTurningDown);
            m_isTurningDown = true;
        }

        void OnEndBackwardThrustEvt(EventPtr evt)
        {
            _ASSERTE(m_isTurningDown);
            m_isTurningDown = false;
        }

        bool m_isTurningRight;
        bool m_isTurningLeft;
        bool m_isTurningUp;
        bool m_isTurningDown;
        real m_turnVelocity;
        ActorID m_actorId;

    };
}