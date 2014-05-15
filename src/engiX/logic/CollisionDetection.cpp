#include "CollisionDetection.h"
#include "MathHelper.h"

using namespace engiX;
using namespace DirectX;

bool BoundingSphere::IsPointInside(_In_ const Vec3& point)
{
     Vec3 distSq;
     
     XMStoreFloat3(&distSq,
         XMVector3LengthSq(XMVectorSubtract(XMLoadFloat3(&point), XMLoadFloat3(&m_position))));

     return (distSq.x <= m_radiusSq);
}
