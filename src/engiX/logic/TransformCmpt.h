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

        real RotationX() const { return m_rotationXYZ.x; }
        real RotationY() const { return m_rotationXYZ.y; }
        const Vec3& Position() const { return m_pos; }
        Vec3 Direction() const;
        void RotationY(_In_ real theta);
        void RotationX(_In_ real theta);
        
        void Position(_In_ const Vec3& newPos);
        void Transform(_In_ const TransformCmpt& tsfm);
        const Mat4x4& Transform() const { return m_transform; }
        const Mat4x4& InverseTransform() const { return m_invTransform; }

        void LookAt(_In_ const Vec3& target);
        void PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi);

    protected:
        void CalcTransform();
        Mat4x4 CalcRotationMat() const;

        Vec3 m_rotationXYZ;
        Vec3 m_pos;

    private:
        Mat4x4 m_transform;
        Mat4x4 m_invTransform;
    };
}