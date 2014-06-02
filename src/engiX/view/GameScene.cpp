#include "GameScene.h"
#include <memory>
#include <DirectXColors.h>
#include "DXUT.h"
#include "EventManager.h"
#include "WinGameApp.h"
#include "RenderComponent.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

GameScene::GameScene() :
    m_pSceneRoot(eNEW RootSceneNode(this)),
    m_pWireframeRS(nullptr)
{
    m_pCameraNodes.push_back(shared_ptr<SceneCameraNode>(eNEW SceneCameraNode(this)));
    m_pCameraNodes.push_back(shared_ptr<SceneCameraNode>(eNEW SceneCameraNode(this)));
    m_pCameraNodes.push_back(shared_ptr<SceneCameraNode>(eNEW SceneCameraNode(this)));
    m_pCameraNodes.push_back(shared_ptr<SceneCameraNode>(eNEW SceneCameraNode(this)));
    m_currCameraIdx = 0;
}

GameScene::~GameScene()
{
    SAFE_DELETE(m_pSceneRoot);
    SAFE_RELEASE(m_pWireframeRS);
}

bool GameScene::Init()
{
    m_pCameraNodes[0]->PlaceOnSphere(25.0, 1.60f * R_PI, 0.45f * R_PI);
    m_pSceneRoot->AddChild(m_pCameraNodes[0]);

    m_pCameraNodes[1]->PlaceOnSphere(25.0, 0.25f * R_PI, 0.25f * R_PI);
    m_pSceneRoot->AddChild(m_pCameraNodes[1]);

    m_pCameraNodes[2]->PlaceOnSphere(125.0, 0.5f * R_PI, 0.01f * R_PI);
    m_pSceneRoot->AddChild(m_pCameraNodes[2]);

    m_pCameraNodes[3]->PlaceOnSphere(100.0, 0.0f, 0.48f * R_PI);
    m_pSceneRoot->AddChild(m_pCameraNodes[3]);

    Mat4x4 identity;
    XMStoreFloat4x4(&identity, XMMatrixIdentity());
    m_worldTransformationStack.push(identity);

    REGISTER_EVT(GameScene, ActorCreatedEvt);
    REGISTER_EVT(GameScene, ActorDestroyedEvt);
    REGISTER_EVT(GameScene, ToggleCameraEvt);

    return true;
}

HRESULT GameScene::OnConstruct()
{
    SAFE_RELEASE(m_pWireframeRS);

    D3D11_RASTERIZER_DESC wireframeDesc;
    ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_BACK;
    wireframeDesc.FrontCounterClockwise = false;
    wireframeDesc.DepthClipEnable = true;

    CHRRHR(DXUTGetD3D11Device()->CreateRasterizerState(&wireframeDesc, &m_pWireframeRS));

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

    DXUTGetD3D11DeviceContext()->RSSetState(m_pWireframeRS);

    if (m_pSceneRoot && m_pCameraNodes[m_currCameraIdx])
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

    WeakActorComponentPtr pWeakRenderer(pActor->Get<RenderComponent>());
    _ASSERTE(!pWeakRenderer.expired());

    shared_ptr<RenderComponent> pRenderer = static_pointer_cast<RenderComponent>(pWeakRenderer.lock());

    auto pSceneNode = pRenderer->CreateSceneNode(this);
    CHRR(pSceneNode->OnConstruct());

    m_pSceneRoot->AddChild(pSceneNode);

    LogInfo("Actor %s[%x] ScenNode created and added to scene root node children", pActor->Typename(), pActor->Id());
}

void GameScene::OnActorDestroyedEvt(_In_ EventPtr pEvt)
{
    shared_ptr<ActorDestroyedEvt> pActrEvt = static_pointer_cast<ActorDestroyedEvt>(pEvt);

    m_pSceneRoot->RemoveChild(pActrEvt->ActorId());
}

void GameScene::OnToggleCameraEvt(_In_ EventPtr pEvt)
{
    LogInfo("Game scene is toggle its camera");
    m_currCameraIdx = (m_currCameraIdx + 1) % m_pCameraNodes.size();
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