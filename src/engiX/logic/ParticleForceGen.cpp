#include <algorithm>
#include "ParticleForceGen.h"
#include "Logger.h"
#include "TransformCmpt.h"
#include "ParticlePhysicsCmpt.h"

using namespace std;
using namespace engiX;
using namespace DirectX;

ParticleForceGenID ParticleForceGen::m_lastId = 0;

ParticleForceGenID ParticleForceRegistry::RegisterGenerator(_In_ std::shared_ptr<ParticleForceGen> pFGen)
{
    LogVerbose("Registering force generator %s[%d]", pFGen->Typename(), pFGen->Id());
    m_forceRegistry.insert(make_pair(pFGen->Id(), pFGen));

    return pFGen->Id();
}

void ParticleForceRegistry::RegisterActorForce(_In_ ActorID actorId, _In_ ParticleForceGenID pfgenId)
{
    _ASSERTE(m_forceRegistry.count(pfgenId) > 0);
    LogVerbose("Adding actor-force registration {%d, %s[%d]}", actorId, m_forceRegistry.at(pfgenId)->Typename(), pfgenId);
    m_actorRegistry[actorId].insert(pfgenId);
}

void ParticleForceRegistry::UnregisterActorForce(_In_ ActorID actorId, _In_ ParticleForceGenID fgenId)
{
    if (m_actorRegistry.count(actorId) == 0)
    {
        LogError("Actor[%d] does not exist, call has no effect", actorId);
        return;
    }

    auto forces = m_actorRegistry[actorId];
    LogVerbose("Removing actor-force registration {%d, %s[%d]}", actorId, m_forceRegistry.at(fgenId)->Typename(), fgenId);
    forces.erase(fgenId);
}

void ParticleAnchoredSpring::ApplyForce(_In_ Actor* pActor, _In_ const Timer& time) const
{
    // d = xa - xb; xa is particle end of spring, xb is the anchor end of spring
    // f = -k(|d| - l0)d; l0 is rest length, k is spring constant
    Vec3 particlePos = pActor->Get<TransformCmpt>().Position();
    XMVECTOR xa = XMLoadFloat3(&particlePos);
    XMVECTOR xb = XMLoadFloat3(&m_anchor);
    XMVECTOR d = XMVectorSubtract(xa, xb);
    XMVECTOR dLen = XMVector3Length(d);
    XMVECTOR k = XMVectorReplicate(-m_sprintConst);
    XMVECTOR l0 = XMVectorReplicate(m_restLength);

    Vec3 f;
    XMStoreFloat3(&f, XMVectorMultiply(d,
        XMVectorMultiply(k,
        XMVectorSubtract(dLen, l0))));

    pActor->Get<ParticlePhysicsCmpt>().AddForce(f);
}
