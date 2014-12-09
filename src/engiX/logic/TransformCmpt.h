#pragma once

#include "engiXDefs.h"
#include "Actor.h"

namespace engiX
{
    class TransformCmpt : public ActorCmpt
    {
    public:
        DECLARE_COMPONENT(TransformCmpt, 0x76EE7B4E);

        TransformCmpt();
        void OnUpdate(_In_ const Timer& time) {}
        bool Init() {  return true; }

        // Setters
        void Position(_In_ const Vec3& newPos);
        void Transform(_In_ const Mat4x4& tsfm) { m_transform = tsfm; }
        void LookAt(_In_ const Vec3& target);
        void PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi);
        void RotateLocal(_In_ real pitchX, _In_ real yawY, _In_ real rollZ);

        // Getters
        const Mat4x4& Transform() const { return m_transform; }
        Mat4x4 InverseTransform() const;
        Vec3 Right() const { Vec3(m_transform.m[0]); }
        Vec3 Up() const { Vec3(m_transform.m[1]); }
        Vec3 Forward() const { Vec3(m_transform.m[2]); }
        Vec3 Position() const { return Vec3(m_transform.m[3]); }

    private:
        Mat4x4 m_transform;
    };
}