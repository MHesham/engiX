#pragma once

#include "engiXDefs.h"

namespace engiX
{
    class BoundingSphere
    {
    public:
        BoundingSphere() :
            m_radius(0.0),
            m_position(DirectX::g_XMZero)
        {}

        BoundingSphere(_In_ real radius, _In_ Vec3 position = Vec3(0.0, 0.0, 0.0)) :
            m_radius(radius),
            m_radiusSq(radius * radius),
            m_position(position)
        {}

        bool IsPointInside(_In_ const Vec3& point);
        bool Collide(_In_ const BoundingSphere& other);
        Vec3 Position() const { return m_position; }
        void Position(Vec3 val) { m_position = val; }
        real Radius() const { return m_radius; }
        void Radius(real val) { m_radius = val; m_radiusSq = val * val; }
        bool IsNull() const { return m_radius == 0.0f; }

    private:
        Vec3 m_position;
        real m_radius;
        real m_radiusSq;
    };
}