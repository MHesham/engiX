#pragma once

#include "Actor.h"
#include "engiXDefs.h"
#include "TransformCmpt.h"
#include "CollisionDetection.h"
#include "MathHelper.h"

namespace engiX
{
    class ParticlePhysicsCmpt : public ActorComponent
    {
    public:
        DECLARE_COMPONENT(ParticlePhysicsCmpt, 0x37C19534);

        static const real DefaultDamping;

        ParticlePhysicsCmpt();
        void OnUpdate(_In_ const Timer& time) { ApplyForces(time); Integrate(time); }
        Vec3 Velocity() const { return m_velocity; }
        void Velocity(_In_ Vec3 val) { m_velocity = val; }
        Vec3 BaseAcceleraiton() const { return m_baseAcceleraiton; }
        void BaseAcceleraiton(_In_ Vec3 val) { m_baseAcceleraiton = val; }
        real Mass() const { return real((m_inverseMass > 0.0) ? 1.0 / m_inverseMass : REAL_MAX); }
        void Mass(_In_ real val) { m_inverseMass = 1.0f / val; }
        void InverseMass(_In_ real val) { m_inverseMass = val; }
        real Damping() const { return m_damping; }
        void Damping(_In_ real val) { m_damping = val; }
        void ScaleVelocity(_In_ real scale);
        void LifetimeBound(_In_ const BoundingSphere& lifetimeBound) { m_lifetimeBound = lifetimeBound; }
        BoundingSphere BoundingMesh() const;
        void Radius(_In_ real radius) { m_radius = radius; }
        void AddForce(_In_ const Vec3& force) { Math::Vec3Accumulate(m_accumulatedForce, force); }

    protected:
        void ApplyForces(_In_ const Timer& time);
        void Integrate(_In_ const Timer& time);
        void ClearForceAccum() { m_accumulatedForce.x = m_accumulatedForce.y = m_accumulatedForce.z = 0.0; }

        Vec3 m_velocity;
        Vec3 m_baseAcceleraiton;
        real m_inverseMass;
        real m_damping;
        real m_radius;

        Vec3 m_accumulatedForce;
        BoundingSphere m_lifetimeBound;
    };
}