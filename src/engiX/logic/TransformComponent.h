#pragma once

#include "engiXDefs.h"
#include "Actor.h"

namespace engiX
{
    class TransformComponent : public ActorComponent
    {
    public:
        static const ComponentID TypeID = 0x76EE7B4E; 
        TransformComponent() { XMStoreFloat4x4(&m_transform, DirectX::XMMatrixIdentity()); }
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"TransformComponent"; }
        void OnUpdate(_In_ const Timer& time) {}
        bool Init() {  return true; }

        Vec3 Position() const { return Vec3(m_transform.m[3][0], m_transform.m[3][1], m_transform.m[3][2]); }
        void Position(const Vec3& newPos)
        {
            m_transform.m[3][0] = newPos.x;
            m_transform.m[3][1] = newPos.y;
            m_transform.m[3][2] = newPos.z;
        }

    protected:
        Mat4x4 m_transform;
    };
}