#pragma once

#include "engiXDefs.h"
#include "Actor.h"

namespace engiX
{
    class TransformCmpt : public ActorComponent
    {
    public:
        DECLARE_COMPONENT(TransformCmpt, 0x76EE7B4E);

        TransformCmpt();
        void OnUpdate(_In_ const Timer& time) {}
        bool Init() {  return true; }

        real RotationY() const { return m_rotationXYZ.y; }
        real RotationX() const { return m_rotationXYZ.x; }
        Mat4x4 InverseTransform() const;
        Vec3 Position() const { return Vec3(m_transform._41, m_transform._42, m_transform._43); }
        Vec3 Direction() const;
        void RotationY(_In_ real theta);
        void RotationX(_In_ real theta);
        
        void Position(_In_ const Vec3& newPos);
        void Transform(_In_ const TransformCmpt& tsfm);
        const Mat4x4& Transform() const { return m_transform; }

    protected:
        void CalcTransform();
        Mat4x4 CalcRotationMat() const;

        Vec3 m_rotationXYZ;
        Mat4x4 m_transform;
    };
}