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
        void Control(_In_ std::weak_ptr<TransformCmpt> actorTsfm) { m_actorTsfm = actorTsfm; }
        void Update(_In_ const Timer& time);

    protected:
        void OnStartTurnRightEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(!m_isTurningRight);
            m_isTurningRight = true;
        }

        void OnEndTurnRightEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(m_isTurningRight);
            m_isTurningRight = false;
        }

        void OnStartTurnLeftEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(!m_isTurningLeft);
            m_isTurningLeft = true;
        }

        void OnEndTurnLeftEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(m_isTurningLeft);
            m_isTurningLeft = false;
        }

        void OnStartForwardThrustEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(!m_isTurningUp);
            m_isTurningUp = true;
        }

        void OnEndForwardThrustEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(m_isTurningUp);
            m_isTurningUp = false;
        }

        void OnStartBackwardThrustEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(!m_isTurningDown);
            m_isTurningDown = true;
        }

        void OnEndBackwardThrustEvt(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(m_isTurningDown);
            m_isTurningDown = false;
        }

        bool m_isTurningRight;
        bool m_isTurningLeft;
        bool m_isTurningUp;
        bool m_isTurningDown;
        real m_turnVelocity;
        std::weak_ptr<TransformCmpt> m_actorTsfm;
    };
}