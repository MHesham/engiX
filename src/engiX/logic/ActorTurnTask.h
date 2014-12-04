#pragma once

#include "Task.h"
#include "Actor.h"
#include "TransformCmpt.h"

namespace engiX
{
    class ActorTurnTask : public Task
    {
    public:
        ActorTurnTask(ActorID actorId, Vec3 turnVelocities) :
            m_actorId(actorId),
            m_turnVelocities(turnVelocities)
        {}
        void OnUpdate(_In_ const Timer& time);

    private:
        Vec3 m_turnVelocities;
        ActorID m_actorId;
    };
}