#include "CollisionDetection.h"
#include "MathHelper.h"

using namespace engiX;
using namespace DirectX;

bool BoundingSphere::IsPointInside(_In_ const Vec3& point)
{
     Vec3 distSq;
     
     // distSq = ||v1 - v2||^2
     XMStoreFloat3(&distSq,
         XMVector3LengthSq(
         XMVectorSubtract(XMLoadFloat3(&point), XMLoadFloat3(&m_position))));

     return (distSq.x <= m_radiusSq);
}

bool BoundingSphere::Collide(_In_ const BoundingSphere& other)
{
    Vec3 dist;

    // dist = ||v1 - v2||
    XMStoreFloat3(&dist,
        XMVector3Length(
        XMVectorSubtract(XMLoadFloat3(&other.m_position), XMLoadFloat3(&m_position))));

    return (dist.x <= (m_radius + other.m_radius));
}

