#pragma once

#include <set>
#include <unordered_map>
#include <memory>
#include "Actor.h"

namespace engiX
{
    typedef unsigned ParticleForceGenID;
    typedef unsigned ParticleForceGenTypeID;

    class ParticleForceGen
    {
    public:
        ParticleForceGen() : m_id(++m_lastId) {}
        virtual ~ParticleForceGen() {}
        virtual ParticleForceGenID Id() const { return m_id; }
        virtual ParticleForceGenTypeID TypeId() const = 0;
        virtual const wchar_t* Typename() const = 0;
        virtual void ApplyForce(_In_ Actor* pActor, _In_ const Timer& time) const = 0;

    private:
        static ParticleForceGenID m_lastId;
        ParticleForceGenID m_id;
    };

    class ParticleForceRegistry
    {
    public:
        typedef std::set<ParticleForceGenID> ForceGenSet;

        ParticleForceGenID RegisterGenerator(_In_ std::shared_ptr<ParticleForceGen> pFGen);
        void RegisterActorForce(_In_ ActorID actorId, _In_ ParticleForceGenID pfgenId);
        void UnregisterActorForce(_In_ ActorID actorId, _In_ ParticleForceGenID fgenId);
        bool ActorHasForces(_In_ ActorID actorId) const { return m_actorRegistry.count(actorId) > 0; }
        const ForceGenSet& GetActorForces(_In_ ActorID actorId) const { return m_actorRegistry.at(actorId); }
        std::shared_ptr<const ParticleForceGen> GetForceGen(_In_ ParticleForceGenID pfgenId) const { return m_forceRegistry.at(pfgenId); }

    protected:
        typedef std::pair<ParticleForceGenID, ActorID> ForceRegistration;
        typedef std::unordered_map<ParticleForceGenID, std::shared_ptr<ParticleForceGen>> ForceGenRegistry;
        typedef std::unordered_map<ActorID, ForceGenSet> ActorRegistry;
        ActorRegistry m_actorRegistry;
        ForceGenRegistry m_forceRegistry;
    };

    class ParticleAnchoredSpring : public ParticleForceGen
    {
    public:
        static const ParticleForceGenTypeID TypeID = 0xCF9D9935;
        ParticleAnchoredSpring(_In_ Vec3 anchor, _In_ real restLength, _In_ real springConst) :
            m_anchor(anchor),
            m_restLength(restLength),
            m_sprintConst(springConst)
        {}

        ParticleForceGenTypeID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"ParticleAnchoredSpring"; }
        void ApplyForce(_In_ Actor* pActor, _In_ const Timer& time) const;

    private:
        Vec3 m_anchor;
        real m_restLength;
        real m_sprintConst;
    };
}