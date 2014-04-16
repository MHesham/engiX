#include "SceneCameraNode.h"
#include "MathHelper.h"
#include "WinGameApp.h"
#include "EventManager.h"

using namespace engiX;
using namespace DirectX;

const real SceneCameraNode::DefaultNearPlane = 1.0f;
const real SceneCameraNode::DefaultFarPlane = 1000.0f;
const real SceneCameraNode::DefaultFovAngle = 0.25 * R_PI;

SceneCameraNode::SceneCameraNode(GameScene* pScene) :
    SceneNode(NullActorID, pScene),
    m_nearPlane(DefaultNearPlane),
    m_farPlane(DefaultFarPlane),
    m_fovAngle(DefaultFovAngle),
    m_pos(0.0, 1.0, 0.0)
{
}

HRESULT SceneCameraNode::OnConstruct()
{
    LogInfo("Display settings changed, updating camera");

    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * R_PI, g_pApp->AspectRatio(), m_nearPlane, m_farPlane);
    XMStoreFloat4x4(&m_viewMat, P);

    return S_OK;
}

Mat4x4 SceneCameraNode::WorldViewProjMatrix() const 
{ 
    //XMStoreFloat4x4(&m_viewProjMat, V * P);

    return m_viewMat;
}

void SceneCameraNode::PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi)
{
    MathHelper::ConvertSphericalToCartesian(radius, theta, phi, m_pos);
}

