#include "SceneCameraNode.h"
#include "MathHelper.h"
#include "WinGameApp.h"
#include "EventManager.h"
#include "GameLogic.h"

using namespace engiX;
using namespace DirectX;

const real SceneCameraNode::DefaultNearPlane = 1.0f;
const real SceneCameraNode::DefaultFarPlane = 1500.0f;
const real SceneCameraNode::DefaultFovAngle = 0.25 * R_PI;

SceneCameraNode::SceneCameraNode(GameScene* pScene) :
SceneNode(NullActorID, pScene),
m_nearPlane(DefaultNearPlane),
m_farPlane(DefaultFarPlane),
m_fovAngle(DefaultFovAngle),
m_targetId(NullActorID)
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

void SceneCameraNode::OnUpdate(_In_ const Timer& time)
{
    XMMATRIX cameraTsfm;
    
    if (m_targetId != NullActorID)
    {
        auto& a = g_pApp->Logic()->GetActor(m_targetId);
        auto& targetTsfm = a.Get<TransformCmpt>();

        cameraTsfm = XMMatrixLookAtLH(
            XMVector3TransformCoord(XMLoadFloat3(&m_pos), XMLoadFloat4x4(&targetTsfm.Transform())),
            XMVector3TransformCoord(XMLoadFloat3(&m_lookat), XMLoadFloat4x4(&targetTsfm.Transform())),
            g_XMIdentityR1);
    }
    else
    {
        cameraTsfm = XMMatrixLookAtLH(XMLoadFloat3(&m_pos), XMLoadFloat3(&m_lookat), g_XMIdentityR1);
    }

    XMStoreFloat4x4(&m_worldTsfm, cameraTsfm);
}

Mat4x4 SceneCameraNode::SceneWorldViewProjMatrix() const
{
    const Mat4x4 sceneWorldTsfm = m_pScene->TopTransformation();

    XMMATRIX world = XMLoadFloat4x4(&sceneWorldTsfm);
    XMMATRIX view = XMLoadFloat4x4(&m_worldTsfm);
    XMMATRIX proj = XMLoadFloat4x4(&m_projMat);

    XMMATRIX xWvp;
    xWvp = world * view * proj;

    Mat4x4 wvp;
    XMStoreFloat4x4(&wvp, xWvp);

    return wvp;
}

void SceneCameraNode::PlaceOnSphere(_In_ real radius, _In_ real theta, _In_ real phi, _In_ Vec3 lookat)
{
    Math::ConvertSphericalToCartesian(radius, theta, phi, m_pos);
    m_lookat = lookat;
}