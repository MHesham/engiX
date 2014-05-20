#pragma once

#include "engiXDefs.h"
#include "Actor.h"

namespace engiX
{
    class TransformComponent : public ActorComponent
    {
    public:
        static const ComponentID TypeID = 0x76EE7B4E; 
        TransformComponent();
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"TransformComponent"; }
        void OnUpdate(_In_ const Timer& time) {}
        bool Init() {  return true; }

        const Mat4x4& Transform() { CalcTsfm(); return m_transform; }
        real RotationY() { CalcTsfm(); return m_rotationXYZ.y; }
        Mat4x4 InverseTransform();
        Vec3 Position() { CalcTsfm(); return m_translation; }

        void RotationY(_In_ real theta);
        void Position(_In_ const Vec3& newPos);
        void FrameTransform(_In_ TransformComponent& tsfm);

    protected:
        void CalcTsfm();

        Vec3 m_translation;
        Mat4x4 m_translationMat;
        Vec3 m_rotationXYZ;
        Mat4x4 m_rotationMat;
        Mat4x4 m_transform;
        Mat4x4 m_frameTransform;
        bool m_isDirty;
    };
}