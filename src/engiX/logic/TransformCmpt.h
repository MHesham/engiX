#pragma once

#include "engiXDefs.h"
#include "Actor.h"

namespace engiX
{
    class TransformCmpt : public ActorComponent
    {
    public:
        static const ComponentID TypeID = 0x76EE7B4E; 
        TransformCmpt();
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"TransformCmpt"; }
        void OnUpdate(_In_ const Timer& time) {}
        bool Init() {  return true; }

        real RotationY() const { return m_rotationXYZ.y; }
        Mat4x4 InverseTransform() const;
        Vec3 Position() const { return Vec3(m_transform._41, m_transform._42, m_transform._43); }
        Vec3 Direction() const;
        void RotationY(_In_ real theta);
        
        void Position(_In_ const Vec3& newPos);
        void LocalTransform(_In_ const TransformCmpt& tsfm);
        const Mat4x4& LocalTransform() const { return m_transform; }
        void ToWorldTransform(_In_ const Mat4x4& toWorld) { m_toWorldTransform = toWorld; }
        const Mat4x4& ToWorldTransform() const { return m_toWorldTransform; }

    protected:
        void CalcTransform();
        Mat4x4 CalcRotationMat() const;

        Vec3 m_rotationXYZ;
        Mat4x4 m_transform;
        Mat4x4 m_toWorldTransform;
    };
}