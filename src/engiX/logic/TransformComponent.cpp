#include "TransformComponent.h"

using namespace engiX;
using namespace DirectX;

TransformComponent::TransformComponent() :
    m_translation(DirectX::g_XMZero),
    m_rotationXYZ(DirectX::g_XMZero),
    m_isDirty(true)
{
    XMStoreFloat4x4(&m_frameTransform, XMMatrixIdentity());
    XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
    XMStoreFloat4x4(&m_rotationMat, XMMatrixIdentity());
    XMStoreFloat4x4(&m_translationMat, XMMatrixIdentity());
}

Mat4x4 TransformComponent::InverseTransform()
{
    CalcTsfm();

    XMMATRIX invTranslate = XMMatrixTranslation(-m_translation.x, -m_translation.y, -m_translation.z);
    XMMATRIX invRotate = XMMatrixTranspose(XMLoadFloat4x4(&m_rotationMat));

    Mat4x4 invXform;
    XMStoreFloat4x4(&invXform, XMMatrixMultiply(invTranslate, invRotate));

    return invXform;
}

void TransformComponent::FrameTransform(TransformComponent& tsfm)
{
    m_frameTransform = tsfm.Transform();
    m_isDirty = true;
}

void TransformComponent::RotationY(real theta)
{
    m_rotationXYZ.y = theta;

    XMStoreFloat4x4(&m_rotationMat,
        XMMatrixRotationAxis(g_XMIdentityR1, m_rotationXYZ.y));

    m_isDirty = true;
}

void TransformComponent::Position(const Vec3& newPos)
{
    m_translation = newPos;
    XMStoreFloat4x4(&m_translationMat,
        XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z));

    m_isDirty = true;
}

void TransformComponent::CalcTsfm()
{
    if (!m_isDirty)
        return;

    // M = R * T * F
    XMStoreFloat4x4(&m_transform,
        XMMatrixMultiply(XMLoadFloat4x4(&m_rotationMat),
        XMMatrixMultiply(XMLoadFloat4x4(&m_translationMat),
        XMLoadFloat4x4(&m_frameTransform))));

    m_isDirty = false;
}

