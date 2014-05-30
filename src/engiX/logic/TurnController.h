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
            m_turnVelocity(1.5f)
        { }

        bool Init();
        void Control(_In_ std::weak_ptr<TransformCmpt> actorTsfm) { m_actorTsfm = actorTsfm; }
        void Update(_In_ const Timer& time);

    protected:
        void OnStartTurnRight(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(!m_isTurningRight);
            m_isTurningRight = true;
        }

        void OnEndTurnRight(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(m_isTurningRight);
            m_isTurningRight = false;
        }

        void OnStartTurnLeft(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(!m_isTurningLeft);
            m_isTurningLeft = true;
        }

        void OnEndTurnLeft(EventPtr evt)
        {
            if (m_actorTsfm.expired())
                return;

            _ASSERTE(m_isTurningLeft);
            m_isTurningLeft = false;
        }

        bool m_isTurningRight;
        bool m_isTurningLeft;
        real m_turnVelocity;
        std::weak_ptr<TransformCmpt> m_actorTsfm;
    };
}