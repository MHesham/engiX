#include "ParticlePhysicsCmpt.h"
#include "WinGameApp.h"
#include "GameLogic.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

const real ParticlePhysicsCmpt::DefaultDamping = 0.9f;

ParticlePhysicsCmpt::ParticlePhysicsCmpt() :
    m_velocity(g_XMZero),
    m_baseAcceleraiton(g_XMZero),
    m_accumulatedForce(g_XMZero),
    m_inverseMass(0.0), // 0 inverse mass = 1 / infinite mass, which means a non movable object
    m_damping(DefaultDamping),
    m_radius(0.0)
{

}

BoundingSphere ParticlePhysicsCmpt::BoundingMesh() const
{
    return BoundingSphere(m_radius, Owner()->Get<TransformCmpt>().Position());
}

void ParticlePhysicsCmpt::ApplyForces(_In_ const Timer& time)
{
    const ParticleForceRegistry& forceRegistry = g_pApp->Logic()->ForceRegistry();

    if (forceRegistry.ActorHasForces(Owner()->Id()))
    {
        auto& actorForces = forceRegistry.GetActorForces(Owner()->Id());
        for (auto pfgenId : actorForces)
            forceRegistry.GetForceGen(pfgenId)->ApplyForce(Owner(), time);
    }
}

void ParticlePhysicsCmpt::Integrate(_In_ const Timer& time)
{
    if (m_inverseMass <= 0.0) return;

    auto& tsfmCmpt = Owner()->Get<TransformCmpt>();

    Vec3 newPos = tsfmCmpt.Position();

    //
    // Work out new position p, where p = p0 + vt
    //
    Math::Vec3ScaledAdd(m_velocity, time.DeltaTime(), newPos);
    tsfmCmpt.Position(newPos);

    //
    // Work out acceleration and velocity for next update
    //
    // 1. Work out the acceleration a, where f = m a
    Vec3 netAcceleration = m_baseAcceleraiton;
    Math::Vec3ScaledAdd(m_accumulatedForce, m_inverseMass, netAcceleration);

    // 2. Work out the velocity v from the acceleration a, where v = v0 + at
    Math::Vec3ScaledAdd(netAcceleration, time.DeltaTime(), m_velocity);

    // 3. Apply drag to velocity to simulate loss of energy
    Math::Vec3AddPow(m_damping, time.DeltaTime(), m_velocity);

    // 4. Clear accumulated force during this update cycle
    ClearForceAccum();

    // 5. Check for particle lifetime in case a bound was set
    if (!m_lifetimeBound.IsNull() &&
        !m_lifetimeBound.IsPointInside(tsfmCmpt.Position()))
    {
        m_pOwner->MarkForRemove();
    }
}

void ParticlePhysicsCmpt::ScaleVelocity(_In_ real scale)
{
    XMStoreFloat3(&m_velocity,
        XMVectorScale(XMLoadFloat3(&m_velocity), scale));
}
