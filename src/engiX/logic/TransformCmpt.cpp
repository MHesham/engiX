#include "TransformCmpt.h"
#include "MathHelper.h"

using namespace engiX;
using namespace DirectX;

TransformCmpt::TransformCmpt() :
m_rotationXYZ(DirectX::g_XMZero)
{
    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
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

    XMStoreFloat4x4(&m_invTransform,
        XMMatrixTranspose(XMLoadFloat4x4(&m_transform)));

    m_transform._41 = m_pos.x;
    m_transform._42 = m_pos.y;
    m_transform._43 = m_pos.z;

    m_invTransform._41 = -m_pos.x;
    m_invTransform._42 = -m_pos.y;
    m_invTransform._43 = -m_pos.z;
}

void TransformCmpt::LookAt(_In_ const Vec3& target)
{
    // Reference: Frank Luna DX11 Chapter 5.6.2 View Space
    XMVECTOR T = XMLoadFloat3(&target);
    XMVECTOR Q = XMLoadFloat3(&m_pos);
    XMVECTOR j = g_XMIdentityR1;

    // Front Z Axis
    XMVECTOR w = XMVector3Normalize(XMVectorSubtract(T, Q));
    // Right X Axis
    XMVECTOR u = XMVector3Normalize(XMVector3Cross(j, w));
    // Up Y Axis
    XMVECTOR v = XMVector3Cross(w, u);

    XMMATRIX m(u, v, w, Q);

}

void TransformCmpt::PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi)
{
    Math::ConvertSphericalToCartesian(radius, theta, phi, m_pos);
    CalcTransform();
}

