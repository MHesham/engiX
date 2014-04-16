#include "D3dGeneratedMeshNode.h"
#include <memory>
#include "WinGameApp.h"
#include "GeneratedMeshComponent.h"

using namespace engiX;
using namespace std;

D3dGeneratedMeshNode::D3dGeneratedMeshNode(ActorID actorId, GameScene* pScene) :
    SceneNode(actorId, pScene),
    m_shader(L"data/fx/default.fxo")
{

}

HRESULT D3dGeneratedMeshNode::OnConstruct()
{
    CHRRHR(SceneNode::OnConstruct());
    CHRRHR(m_shader.OnConstruct());

    return S_OK;
}

HRESULT D3dGeneratedMeshNode::OnPreRender()
{
    CHRRHR(SceneNode::OnPreRender());
    CHRRHR(m_shader.OnPreRender(this));

    return S_OK;
}

void D3dGeneratedMeshNode::OnRender()
{
    _ASSERTE(g_pApp->Logic());
    StrongActorPtr pActor((g_pApp->Logic()->GetActor(m_actorId)));
    _ASSERTE(pActor);

    shared_ptr<GeneratedMeshComponent> pMeshCmpt(pActor->GetComponent<GeneratedMeshComponent>());
    _ASSERTE(pMeshCmpt);

    UINT stride = sizeof(D3D11Vertex_PositionColored);
    UINT offset = 0;

    DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11Buffer* pVB = pMeshCmpt->VertextBuffer();
    DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
    DXUTGetD3D11DeviceContext()->IASetIndexBuffer(pMeshCmpt->IndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    DXUTGetD3D11DeviceContext()->DrawIndexed((UINT)pMeshCmpt->IndexCount(), 0, 0);
}