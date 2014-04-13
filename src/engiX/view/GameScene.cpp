#include "GameScene.h"
#include "DXUT.h"
#include <DirectXColors.h>
#include "EventManager.h"
#include <memory>

using namespace engiX;
using namespace std;
using namespace DirectX;

GameScene::GameScene()
{
    m_pCameraNode = shared_ptr<SceneCameraNode>(eNEW SceneCameraNode);
}

bool GameScene::Init()
{
    m_pCameraNode->PlaceOnSphere(500.0, 0, 0.5 * R_PI);

    return true;
}

void GameScene::OnConstruct()
{

}

void GameScene::OnUpdate(_In_ const Timer& time)
{

}

void GameScene::OnRender()
{
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    DXUTGetD3D11DeviceContext()->ClearRenderTargetView(pRTV, Colors::LightBlue);
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    DXUTGetD3D11DeviceContext()->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);
}

void GameScene::OnToggleCamera(EventPtr pEvt)
{
}