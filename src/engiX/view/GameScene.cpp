#include "GameScene.h"
#include <memory>
#include <DirectXColors.h>
#include "DXUT.h"
#include "EventManager.h"
#include "WinGameApp.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

GameScene::GameScene() :
    m_pCameraNode(eNEW SceneCameraNode(this)),
    m_pSceneRoot(eNEW RootSceneNode(this)),
    pActorCreatedHdlr(this, &GameScene::OnActorCreated)
{
}

GameScene::~GameScene()
{
    SAFE_DELETE(m_pSceneRoot);
    SAFE_DELETE(m_pCameraNode);
}

bool GameScene::Init()
{
    m_pCameraNode->PlaceOnSphere(500.0, 0, 0.5 * R_PI);

    Mat4x4 identity;
    XMStoreFloat4x4(&identity, XMMatrixIdentity());
    m_worldTransformationStack.push(identity);

    g_EventMgr->Register(&pActorCreatedHdlr, ActorCreatedEvt::TypeID);

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
    DXUTGetD3D11DeviceContext()->ClearRenderTargetView(pRTV, Colors::LightBlue);
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    DXUTGetD3D11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

    if (m_pSceneRoot && m_pCameraNode)
    {
        // Make sure that the stack has only the identify transformation
        _ASSERTE(m_worldTransformationStack.size() == 1);

        _ASSERTE(m_pSceneRoot);
        if (SUCCEEDED(m_pSceneRoot->OnPreRender()))
        {
            m_pSceneRoot->OnRender();
            m_pSceneRoot->RenderChildren();
            m_pSceneRoot->OnPostRender();
        }
    }
}

void GameScene::OnActorCreated(_In_ EventPtr pEvt)
{
    shared_ptr<ActorCreatedEvt> pActrEvt = static_pointer_cast<ActorCreatedEvt>(pEvt);

    WeakActorPtr pWeakActor(g_pApp->Logic()->FindActor(pActrEvt->ActorId()));
    if (pWeakActor.expired())
    {
        LogWarning("Actor[%x] no longe exist, failed to find it", pActrEvt->ActorId());
        return;
    }

    StrongActorPtr pActor = pWeakActor.lock();

    LogInfo("Actor Created, What to do!");
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