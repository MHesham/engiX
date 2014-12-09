#include "TransformCmpt.h"
#include "MathHelper.h"

using namespace engiX;
using namespace DirectX;

TransformCmpt::TransformCmpt()
{
    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
}

void TransformCmpt::Position(_In_ const Vec3& newPos)
{
    m_transform._41 = newPos.x;
    m_transform._42 = newPos.y;
    m_transform._43 = newPos.z;
}

void TransformCmpt::LookAt(_In_ const Vec3& target)
{
    // Reference: Frank Luna DX11 Chapter 5.6.2 View Space
    XMVECTOR T = XMLoadFloat3(&target);
    Vec3 pos(m_transform.m[3]);
    XMVECTOR Q = XMLoadFloat3(&pos);
    XMVECTOR j = g_XMIdentityR1;

    // Front Z Axis
    XMVECTOR w = XMVector3Normalize(XMVectorSubtract(T, Q));
    // Right X Axis
    XMVECTOR u = XMVector3Normalize(XMVector3Cross(j, w));
    // Up Y Axis
    XMVECTOR v = XMVector3Cross(w, u);

    XMMATRIX m(u, v, w, Q);
    XMStoreFloat4x4(&m_transform, m);
}

void TransformCmpt::PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi)
{
    Vec3 pos;
    Math::ConvertSphericalToCartesian(radius, theta, phi, pos);
    m_transform._41 = pos.x;
    m_transform._42 = pos.y;
    m_transform._43 = pos.z;
}

Mat4x4 TransformCmpt::InverseTransform() const
{
    Mat4x4 invTsfm;
    XMMATRIX xInv = XMLoadFloat4x4(&m_transform);
    XMVECTOR u = xInv.r[0];
    XMVECTOR v = xInv.r[1];
    XMVECTOR w = xInv.r[2];
    XMVECTOR Q = xInv.r[3];

    // Zero the translation from 4th row and
    // keep the 1 at the diagonal
    xInv.r[3] = g_XMIdentityR3;
    // Transpose the rotation matrix
    xInv = XMMatrixTranspose(xInv);

    XMStoreFloat4x4(&invTsfm, xInv);

    // Set inverse X pos
    XMStoreFloat(&invTsfm._41, XMVectorMultiply(Q, u));
    invTsfm._41 *= -1.0f;

    // Set inverse Y poss
    XMStoreFloat(&invTsfm._42, XMVectorMultiply(Q, v));
    invTsfm._42 *= -1.0f;

    // Set inverse Z pos
    XMStoreFloat(&invTsfm._43, XMVectorMultiply(Q, w));
    invTsfm._43 *= -1.0f;

    return invTsfm;
}

void TransformCmpt::RotateLocal(_In_ real pitchX, _In_ real yawY, _In_ real rollZ)
{
    XMMATRIX deltaRotMat = XMMatrixRotationRollPitchYaw(pitchX, yawY, rollZ);
    XMMATRIX currTsfm = XMLoadFloat4x4(&m_transform);
    XMMATRIX newTsfm = XMMatrixMultiply(currTsfm, deltaRotMat);

    XMStoreFloat4x4(&m_transform, newTsfm);
}