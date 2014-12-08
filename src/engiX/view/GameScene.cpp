#include "GameScene.h"
#include <memory>
#include <DirectXColors.h>
#include "DXUT.h"
#include "EventManager.h"
#include "WinGameApp.h"
#include "RenderCmpt.h"
#include "GameLogic.h"
#include "CameraCmpt.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

GameScene::GameScene() :
    m_pSceneRoot(eNEW RootSceneNode(this))
{

}

GameScene::~GameScene()
{
    SAFE_DELETE(m_pSceneRoot);
}

bool GameScene::Init()
{
    Mat4x4 identity;
    XMStoreFloat4x4(&identity, XMMatrixIdentity());
    m_worldTransformationStack.push(identity);

    REGISTER_EVT(GameScene, ActorCreatedEvt);
    REGISTER_EVT(GameScene, ActorDestroyedEvt);

    return true;
}

HRESULT GameScene::OnConstruct()
{
    _ASSERTE(m_pSceneRoot);
    return m_pSceneRoot->OnConstruct();
}

void GameScene::OnUpdate(_In_ const Timer& time)
{
    _ASSERTE(m_pSceneRoot);
    m_pSceneRoot->OnUpdate(time);
}

void GameScene::OnRender()
{
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    DXUTGetD3D11DeviceContext()->ClearRenderTargetView(pRTV, DirectX::Colors::LightBlue);
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    DXUTGetD3D11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

    if (m_pSceneRoot && m_cameraId != NullActorID)
    {
        // Make sure that the stack has only the identify transformation
        _ASSERTE(m_worldTransformationStack.size() == 1);

        _ASSERTE(m_pSceneRoot);
        if (SUCCEEDED(m_pSceneRoot->OnPreRender()))
        {
            m_pSceneRoot->OnRender();
            m_pSceneRoot->OnPostRender();
        }
        m_pSceneRoot->RenderChildren();

    }
}

void GameScene::OnActorCreatedEvt(_In_ EventPtr pEvt)
{
    shared_ptr<ActorCreatedEvt> pActrEvt = static_pointer_cast<ActorCreatedEvt>(pEvt);

    auto& a = g_pApp->Logic()->GetActor(pActrEvt->ActorId());
    _ASSERTE(!a.IsNull());

    auto &renderCmpt = a.Get<MeshCmpt>();

    auto pSceneNode = renderCmpt.CreateSceneNode(this);
    renderCmpt.SceneNode(pSceneNode);
    CHRR(pSceneNode->OnConstruct());

    m_pSceneRoot->AddChild(pSceneNode);

    LogVerbose("Actor %s[%x] ScenNode created and added to scene root node children", a.Typename(), a.Id());
}

void GameScene::OnActorDestroyedEvt(_In_ EventPtr pEvt)
{
    shared_ptr<ActorDestroyedEvt> pActrEvt = static_pointer_cast<ActorDestroyedEvt>(pEvt);

    m_pSceneRoot->RemoveChild(pActrEvt->ActorId());
}

void GameScene::PushTransformation(_In_ const Mat4x4& t)
{
    XMMATRIX topMat = XMLoadFloat4x4(&m_worldTransformationStack.top());
    XMMATRIX newTransMat = XMLoadFloat4x4(&t);

    // Duplicate the matrix on top
    m_worldTransformationStack.push(m_worldTransformationStack.top());
    // Replace the top matrix with the new transformation
    XMStoreFloat4x4(&m_worldTransformationStack.top(), newTransMat * topMat);
}

void GameScene::PopTransformation()
{ 
    // The identity transform should always be there and no one
    // should ever pop it if the scene rendering logic is correct
    _ASSERTE(m_worldTransformationStack.size() > 1);
    m_worldTransformationStack.pop(); 
}

Mat4x4 GameScene::CameraWorldViewProjMatrix() const
{
    const Mat4x4 sceneWorldTsfm = TopTransformation();
    auto& cA = g_pApp->Logic()->GetActor(m_cameraId);
    auto& cameraCmpt = cA.Get<CameraCmpt>();

    XMMATRIX world = XMLoadFloat4x4(&sceneWorldTsfm);
    XMMATRIX view = XMLoadFloat4x4(&cameraCmpt.ViewTsfm());
    XMMATRIX proj = XMLoadFloat4x4(&cameraCmpt.ProjTsfm());

    XMMATRIX xWvp;
    xWvp = world * view * proj;

    Mat4x4 wvp;
    XMStoreFloat4x4(&wvp, xWvp);

    return wvp;
}