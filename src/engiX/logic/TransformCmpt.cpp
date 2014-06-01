#include "TransformCmpt.h"

using namespace engiX;
using namespace DirectX;

TransformCmpt::TransformCmpt() :
    m_rotationXYZ(DirectX::g_XMZero)
{
    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
    XMStoreFloat4x4(&m_toWorldTransform, XMMatrixIdentity());
}

Mat4x4 TransformCmpt::InverseTransform() const
{
    Mat4x4 rotMat = CalcRotationMat();
    Mat4x4 invTsfm;
    
    XMStoreFloat4x4(&invTsfm,
        XMMatrixTranspose(XMLoadFloat4x4(&rotMat)));

    invTsfm._41 = -m_transform._41;
    invTsfm._42 = -m_transform._42;
    invTsfm._43 = -m_transform._43;

    return invTsfm;
}

Mat4x4 TransformCmpt::CalcRotationMat() const
{
    Mat4x4 rotMat;

    XMStoreFloat4x4(&rotMat,
        XMMatrixRotationY(m_rotationXYZ.y));

    return rotMat;
}

Vec3 TransformCmpt::Direction() const
{
    Mat4x4 rotMat = m_transform;
    rotMat._41 = rotMat._42 = rotMat._43 = 0.0f;

    Vec3 dir;

    XMStoreFloat3(&dir,
        XMVector3Transform(g_XMIdentityR2,
        XMLoadFloat4x4(&rotMat)));

    return dir;
}

void TransformCmpt::LocalTransform(_In_ const TransformCmpt& tsfm)
{
    m_rotationXYZ = tsfm.m_rotationXYZ;
    m_transform = tsfm.m_transform;
}

void TransformCmpt::RotationY(_In_ real theta)
{
    m_rotationXYZ.y = theta;
    CalcTransform();
}

void TransformCmpt::Position(_In_ const Vec3& newPos)
{
   m_transform._41 = newPos.x;
   m_transform._42 = newPos.y;
   m_transform._43 = newPos.z;
}

void TransformCmpt::CalcTransform()
{
    Vec3 pos = Position();

    m_transform = CalcRotationMat();

    Position(pos);
}

