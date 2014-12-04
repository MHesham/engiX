#include "GameScene.h"
#include <memory>
#include <DirectXColors.h>
#include "DXUT.h"
#include "EventManager.h"
#include "WinGameApp.h"
#include "RenderComponent.h"
#include "GameLogic.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

GameScene::GameScene() :
    m_pSceneRoot(eNEW RootSceneNode(this)),
    m_currCameraIdx(-1)
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
    REGISTER_EVT(GameScene, ToggleCameraEvt);

    return true;
}

shared_ptr<SceneCameraNode> GameScene::Camera()
{
    if (m_cameras.empty())
    {
        LogWarning("Scene camera was requsted and there is no one set yet, will create a default one");
        AddCamera();
    }

    return m_cameras[m_currCameraIdx]; 
}

shared_ptr<SceneCameraNode> GameScene::AddCamera()
{
    shared_ptr<SceneCameraNode> pCamera(eNEW SceneCameraNode(this));

    // In case this is the first camera to add, then set the camera idx to it
    if (m_cameras.empty())
        m_currCameraIdx = 0;

    m_cameras.push_back(pCamera);
    m_pSceneRoot->AddChild(pCamera);

    return pCamera;
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

    if (m_pSceneRoot && m_cameras[m_currCameraIdx])
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

    WeakActorPtr pWeakActor(g_pApp->Logic()->FindActor(pActrEvt->ActorId()));
    _ASSERTE(!pWeakActor.expired());

    StrongActorPtr pActor = pWeakActor.lock();

    auto &renderCmpt = pActor->Get<RenderComponent>();


    auto pSceneNode = renderCmpt.CreateSceneNode(this);
    renderCmpt.SceneNode(pSceneNode);
    CHRR(pSceneNode->OnConstruct());

    m_pSceneRoot->AddChild(pSceneNode);

    LogVerbose("Actor %s[%x] ScenNode created and added to scene root node children", pActor->Typename(), pActor->Id());
}

void GameScene::OnActorDestroyedEvt(_In_ EventPtr pEvt)
{
    shared_ptr<ActorDestroyedEvt> pActrEvt = static_pointer_cast<ActorDestroyedEvt>(pEvt);

    m_pSceneRoot->RemoveChild(pActrEvt->ActorId());
}

void GameScene::OnToggleCameraEvt(_In_ EventPtr pEvt)
{
    LogInfo("Game scene toggle its camera");
    m_currCameraIdx = (m_currCameraIdx + 1) % m_cameras.size();
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