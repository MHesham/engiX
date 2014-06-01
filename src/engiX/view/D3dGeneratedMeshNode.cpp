#include "D3dGeneratedMeshNode.h"
#include <memory>
#include "WinGameApp.h"
#include "RenderComponent.h"

using namespace engiX;
using namespace std;

D3dGeneratedMeshNode::D3dGeneratedMeshNode(_In_ ActorID actorId, _In_ const GeometryGenerator::MeshData& mesh,  _In_ Color3 color, _In_ GameScene* pScene) :
    SceneNode(actorId, pScene),
    m_pIndexBuffer(nullptr),
    m_pVertexBuffer(nullptr),
    m_shader(L"data/fx/default.fxo")
{
    m_vertices.resize(mesh.Vertices.size());

    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        m_vertices[i].Position = mesh.Vertices[i].Position;
        m_vertices[i].Color = color;
    }

    m_indices.assign(mesh.Indices.begin(), mesh.Indices.end());
}

D3dGeneratedMeshNode::~D3dGeneratedMeshNode()
{
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);
}

HRESULT D3dGeneratedMeshNode::OnConstruct()
{
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);

    CHRRHR(SceneNode::OnConstruct());
    CHRRHR(m_shader.OnConstruct());
    CHRRHR(D3dShader::CreateVertexBufferFrom(&m_vertices[0], m_vertices.size(), m_pVertexBuffer));
    CHRRHR(D3dShader::CreateIndexBufferFrom(&m_indices[0], m_indices.size(), m_pIndexBuffer));

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
    if (m_actor.expired())
        return;

    StrongActorPtr pActor(m_actor);
    _ASSERTE(pActor);

    shared_ptr<RenderComponent> pMeshCmpt(pActor->Get<RenderComponent>());
    _ASSERTE(pMeshCmpt);

    UINT stride = sizeof(D3D11Vertex_PositionColored);
    UINT offset = 0;

    _ASSERTE(m_pVertexBuffer);
    _ASSERTE(m_pIndexBuffer);

    DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    
    DXUTGetD3D11DeviceContext()->DrawIndexed((UINT)IndexCount(), 0, 0);
}