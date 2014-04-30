#include "ParticlePhysicsComponent.h"
#include "WinGameApp.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

const real ParticlePhysicsComponent::DefaultDamping = 0.9f;

ParticlePhysicsComponent::ParticlePhysicsComponent() :
    m_velocity(g_XMZero),
    m_baseAcceleraiton(g_XMZero),
    m_accumulatedForce(g_XMZero),
    m_inverseMass(0.0), // 0 inverse mass = 1 / infinite mass, which means a non movable object
    m_damping(DefaultDamping)
{

}

bool ParticlePhysicsComponent::Init()
{
    WeakActorPtr pActor = g_pApp->Logic()->FindActor(Owner());
    CBRB(!pActor.expired());

    m_pObjTsfm = pActor.lock()->GetComponent<TransformComponent>();

    return true;
}

void ParticlePhysicsComponent::Integrate(_In_ const Timer& time)
{
    if (m_inverseMass <= 0.0) return;

    _ASSERTE(!m_pObjTsfm.expired());
    shared_ptr<TransformComponent> pTsfmCmpt = m_pObjTsfm.lock();

    Vec3 newPos = pTsfmCmpt->Position();
    // Work out new position p, where p = p0 + vt
    AddScaledVector(m_velocity, time.DeltaTime(), newPos);
    pTsfmCmpt->Position(newPos);

    // Work out acceleration and velocity for next update

    // 1. Work out the acceleration a, where f = m a
    Vec3 netAcceleration = m_baseAcceleraiton;
    AddScaledVector(m_accumulatedForce, m_inverseMass, netAcceleration);
    // 2. Work out the velocity V from the acceleration A, where v = v0 + at
    AddScaledVector(netAcceleration, time.DeltaTime(), m_velocity);

    // 3. Apply drag to velocity to simulate loss of energy
    AddPowVector(m_damping, time.DeltaTime(), m_velocity);

    // 4. Clear accumulated force during this update cycle
    m_accumulatedForce.x = m_accumulatedForce.y = m_accumulatedForce.z = 0.0;
}

void ParticlePhysicsComponent::AddScaledVector(_In_ const Vec3& vec, _In_ real scale, _Inout_ Vec3& res)
{
    // res = res + vec * scale
    XMStoreFloat3(&res,
        XMVectorMultiplyAdd(XMLoadFloat3(&vec), XMVectorReplicate(scale), XMLoadFloat3(&res)));
}

void ParticlePhysicsComponent::AddPowVector(_In_ const real& a, _In_ real b, _Inout_ Vec3& res)
{
    // res = res + a^b
    XMStoreFloat3(&res,
        XMVectorMultiply(XMLoadFloat3(&res), XMVectorReplicate(real_pow(a, b))));
}
