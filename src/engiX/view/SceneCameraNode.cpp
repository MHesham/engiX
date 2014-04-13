#include "SceneCameraNode.h"
#include "MathHelper.h"
#include "WinGameApp.h"
#include "EventManager.h"

using namespace engiX;
using namespace DirectX;

const real SceneCameraNode::DefaultNearPlane = 1.0f;
const real SceneCameraNode::DefaultFarPlane = 1000.0f;
const real SceneCameraNode::DefaultFovAngle = 0.25 * R_PI;

SceneCameraNode::SceneCameraNode() :
    m_nearPlane(DefaultNearPlane),
    m_farPlane(DefaultFarPlane),
    m_fovAngle(DefaultFovAngle),
    m_pos(0.0, 1.0, 0.0)
{
}

void SceneCameraNode::OnConstruct()
{
    LogInfo("Display settings changed, updating camera");
    BuildViewProjMatrix();
}

void SceneCameraNode::BuildViewProjMatrix()
{
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * R_PI, g_pApp->AspectRatio(), m_nearPlane, m_farPlane);
    
    XMVECTOR pos    = XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX V = XMMatrixLookAtLH(pos, target, up);

    XMStoreFloat4x4(&m_viewProjMat, V * P);
}

void SceneCameraNode::PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi)
{
    MathHelper::ConvertSphericalToCartesian(radius, theta, phi, m_pos);
    BuildViewProjMatrix();
}

