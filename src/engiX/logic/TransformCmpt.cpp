#include "TransformCmpt.h"

using namespace engiX;
using namespace DirectX;

TransformCmpt::TransformCmpt() :
m_rotationXYZ(DirectX::g_XMZero)
{
    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
}

Mat4x4 TransformCmpt::InverseTransform() const
{
    Mat4x4 rotMat = CalcRotationMat();
    Mat4x4 invTsfm;

    XMStoreFloat4x4(&invTsfm,
        XMMatrixTranspose(XMLoadFloat4x4(&rotMat)));

    invTsfm._41 = -m_pos.x;
    invTsfm._42 = -m_pos.y;
    invTsfm._43 = -m_pos.z;

    return invTsfm;
}

Mat4x4 TransformCmpt::CalcRotationMat() const
{
    Mat4x4 rotMat;

    XMStoreFloat4x4(&rotMat,
        XMMatrixMultiply(
        XMMatrixRotationX(m_rotationXYZ.x),
        XMMatrixRotationY(m_rotationXYZ.y)));

    return rotMat;
}

Vec3 TransformCmpt::Direction() const
{
    Mat4x4 rotMat = Transform();
    rotMat._41 = rotMat._42 = rotMat._43 = 0.0f;

    Vec3 dir;

    XMStoreFloat3(&dir,
        XMVector3Transform(g_XMIdentityR2,
        XMLoadFloat4x4(&rotMat)));

    return dir;
}

void TransformCmpt::Transform(_In_ const TransformCmpt& tsfm)
{
    m_rotationXYZ = tsfm.m_rotationXYZ;
    m_pos = tsfm.m_pos;
    CalcTransform();
}

void TransformCmpt::RotationY(_In_ real theta)
{
    m_rotationXYZ.y = theta;
    CalcTransform();
}

void TransformCmpt::RotationX(_In_ real theta)
{
    m_rotationXYZ.x = theta;
    CalcTransform();
}

void TransformCmpt::Position(_In_ const Vec3& newPos)
{
    m_pos = newPos;
    CalcTransform();
}

void TransformCmpt::CalcTransform()
{
    m_transform = CalcRotationMat();

    m_transform._41 = m_pos.x;
    m_transform._42 = m_pos.y;
    m_transform._43 = m_pos.z;
}

