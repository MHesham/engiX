#pragma once

#include "Actor.h"
#include "engiXDefs.h"
#include "TransformComponent.h"

namespace engiX
{
    class ParticlePhysicsComponent : public ActorComponent
    {
    public:
        static const real DefaultDamping;
        static const ComponentID TypeID = 0x37C19534;

        ParticlePhysicsComponent();
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"ParticlePhysicsComponent"; }
        void OnUpdate(_In_ const Timer& time) { Integrate(time); }
        bool Init();
        Vec3 Velocity() const { return m_velocity; }
        void Velocity(Vec3 val) { m_velocity = val; }
        Vec3 BaseAcceleraiton() const { return m_baseAcceleraiton; }
        void BaseAcceleraiton(Vec3 val) { m_baseAcceleraiton = val; }
        real Mass() const { return real((m_inverseMass > 0.0) ? 1.0 / m_inverseMass : REAL_MAX); }
        void InverseMass(real val) { m_inverseMass = val; }
        real Damping() const { return m_damping; }
        void Damping(real val) { m_damping = val; }
        
        // res = res + vec * scale
        static void AddScaledVector(_In_ const Vec3& vec, _In_ real scale, _Inout_ Vec3& res);
        // res = res + a^b
        static void AddPowVector(_In_ const real& a, _In_ real b, _Inout_ Vec3& res);

    protected:
        void Integrate(_In_ const Timer& time);
        Vec3 m_velocity;
        Vec3 m_baseAcceleraiton;
        Vec3 m_accumulatedForce;
        real m_inverseMass;
        real m_damping;
        std::weak_ptr<TransformComponent> m_pObjTsfm;
    };
}