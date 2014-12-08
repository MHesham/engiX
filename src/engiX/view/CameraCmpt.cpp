#include "CameraCmpt.h"
#include "MathHelper.h"
#include "WinGameApp.h"
#include "EventManager.h"
#include "GameLogic.h"
#include "SceneCameraNode.h"

using namespace engiX;
using namespace DirectX;
using namespace std;

const real CameraCmpt::DefaultNearPlane = 1.0f;
const real CameraCmpt::DefaultFarPlane = 1500.0f;
const real CameraCmpt::DefaultFovAngle = 0.25 * R_PI;

CameraCmpt::CameraCmpt() :
m_nearPlane(DefaultNearPlane),
m_farPlane(DefaultFarPlane),
m_fovAngle(DefaultFovAngle),
m_targetId(NullActorID)
{
    XMStoreFloat4x4(&m_viewTsfm, XMMatrixIdentity());
    XMStoreFloat4x4(&m_projTsfm, XMMatrixIdentity());

    REGISTER_EVT(CameraCmpt, DisplaySettingsChangedEvt);
}

void CameraCmpt::OnDisplaySettingsChangedEvt(EventPtr pEvt)
{
    LogInfo("Display settings changed, updating camera");

    real aspectRatio = g_pApp->AspectRatio();
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * R_PI, aspectRatio, m_nearPlane, m_farPlane);
    XMStoreFloat4x4(&m_projTsfm, P);
}

void CameraCmpt::OnUpdate(_In_ const Timer& time)
{
    XMMATRIX cameraTsfm;

    auto& aT = Owner()->Get<TransformCmpt>();

    if (m_targetId != NullActorID)
    {
        auto& a = g_pApp->Logic()->GetActor(m_targetId);
        auto& targetTsfm = a.Get<TransformCmpt>();

        XMStoreFloat4x4(&m_viewTsfm, XMMatrixMultiply(
            XMLoadFloat4x4(&aT.InverseTransform()),
            XMLoadFloat4x4(&targetTsfm.InverseTransform())));
    }
    else
    {
        m_viewTsfm = aT.InverseTransform();
    }
}

shared_ptr<ISceneNode> CameraCmpt::CreateSceneNode(_In_ GameScene* pScene)
{
    return shared_ptr<ISceneNode>(eNEW SceneCameraNode(Owner()->Id(), pScene));
}