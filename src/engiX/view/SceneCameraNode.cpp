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
    m_fovAngle(DefaultFovAngle)
{
    XMStoreFloat4x4(&m_projMat, XMMatrixIdentity());
}

HRESULT SceneCameraNode::OnConstruct()
{
    LogInfo("Display settings changed, updating camera");

    real aspectRatio = g_pApp->AspectRatio();
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * R_PI, aspectRatio, m_nearPlane, m_farPlane);
    XMStoreFloat4x4(&m_projMat, P);

    return S_OK;
}

Mat4x4 SceneCameraNode::SceneWorldViewProjMatrix() const 
{ 
    const Mat4x4 sceneWorldTsfm = m_pScene->TopTransformation();

    XMMATRIX world = XMLoadFloat4x4(&sceneWorldTsfm);
    XMMATRIX view = XMLoadFloat4x4(&m_worldTsfm);
    XMMATRIX proj = XMLoadFloat4x4(&m_projMat);

    XMMATRIX xWvp = world * view * proj;

    Mat4x4 wvp;
    XMStoreFloat4x4(&wvp, xWvp);

    return wvp;
}

void SceneCameraNode::PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi)
{
    Vec3 pos;
    Math::ConvertSphericalToCartesian(radius, theta, phi, pos);
    FXMVECTOR xPos = XMLoadFloat3(&pos);

    XMMATRIX lookat = XMMatrixLookAtLH(xPos, g_XMZero, g_XMIdentityR1);

    XMStoreFloat4x4(&m_worldTsfm, lookat);
}

