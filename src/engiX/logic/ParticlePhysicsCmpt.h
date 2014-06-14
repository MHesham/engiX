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
        static const real DefaultDamping;
        static const ComponentID TypeID = 0x37C19534;

        ParticlePhysicsCmpt();
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"ParticlePhysicsCmpt"; }
        void OnUpdate(_In_ const Timer& time) { ApplyForces(time); Integrate(time); }
        bool Init();
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
        Vec3 m_accumulatedForce;
        real m_inverseMass;
        real m_damping;
        std::weak_ptr<TransformCmpt> m_pObjTsfm;
        BoundingSphere m_lifetimeBound;
        real m_radius;
    };
}