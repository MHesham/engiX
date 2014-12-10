#include "D3dGeneratedMeshNode.h"
#include <memory>
#include "WinGameApp.h"
#include "RenderCmpt.h"
#include "GameLogic.h"

using namespace engiX;
using namespace std;

D3dGeneratedMeshNode::D3dGeneratedMeshNode(_In_ ActorID actorId, _In_ const GeometryGenerator::MeshData& mesh,  _In_ Color3 color, _In_ GameScene* pScene) :
    SceneNode(actorId, pScene),
    m_pIndexBuffer(nullptr),
    m_pVertexBuffer(nullptr),
    m_pDbgIndexBuffer(nullptr),
    m_pDbgVertexBuffer(nullptr),
    m_pRasterizeState(nullptr),
    m_shader(L"data/fx/default.fxo")
{
    m_vertices.resize(mesh.Vertices.size());

    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        m_vertices[i].Position = mesh.Vertices[i].Position;
        m_vertices[i].Color = color;
    }

    m_indices.assign(mesh.Indices.begin(), mesh.Indices.end());

    D3D11Vertex_PositionColored dbgV;
    // Front Blue
    dbgV.Position = Vec3(0.0f, 0.0f, 0.0f);
    dbgV.Color = Color3(DirectX::Colors::Blue);
    m_dbgVertices.push_back(dbgV);
    dbgV.Position = Vec3(0.0f, 0.0f, 3.0f);
    dbgV.Color = Color3(DirectX::Colors::Blue);
    m_dbgVertices.push_back(dbgV);

    // Right Red
    dbgV.Position = Vec3(0.0f, 0.0f, 0.0f);
    dbgV.Color = Color3(DirectX::Colors::Red);
    m_dbgVertices.push_back(dbgV);
    dbgV.Position = Vec3(3.0f, 0.0f, 0.0f);
    dbgV.Color = Color3(DirectX::Colors::Red);
    m_dbgVertices.push_back(dbgV);

    // Up Green
    dbgV.Position = Vec3(0.0f, 0.0f, 0.0f);
    dbgV.Color = Color3(DirectX::Colors::Green);
    m_dbgVertices.push_back(dbgV);
    dbgV.Position = Vec3(0.0f, 3.0f, 0.0f);
    dbgV.Color = Color3(DirectX::Colors::Green);
    m_dbgVertices.push_back(dbgV);

    m_dbgIndices.resize(m_dbgVertices.size());
    for (size_t i = 0; i < m_dbgIndices.size(); ++i)
        m_dbgIndices[i] = i;

    ZeroMemory(&m_rasterizeDesc, sizeof(D3D11_RASTERIZER_DESC));
    m_rasterizeDesc.FillMode = (mesh.IsWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID);
    m_rasterizeDesc.CullMode = (mesh.IsBackfacing? D3D11_CULL_FRONT : D3D11_CULL_BACK);
    m_rasterizeDesc.FrontCounterClockwise = false;
    m_rasterizeDesc.DepthClipEnable = true;
}

D3dGeneratedMeshNode::~D3dGeneratedMeshNode()
{
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pDbgIndexBuffer);
    SAFE_RELEASE(m_pDbgVertexBuffer);
    SAFE_RELEASE(m_pRasterizeState);
}

HRESULT D3dGeneratedMeshNode::OnConstruct()
{
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pDbgIndexBuffer);
    SAFE_RELEASE(m_pDbgVertexBuffer);
    SAFE_RELEASE(m_pRasterizeState);

    CHRRHR(SceneNode::OnConstruct())
    CHRRHR(m_shader.OnConstruct());
    CHRRHR(D3dShader::CreateVertexBufferFrom(&m_vertices[0], m_vertices.size(), m_pVertexBuffer));
    CHRRHR(D3dShader::CreateIndexBufferFrom(&m_indices[0], m_indices.size(), m_pIndexBuffer));
    CHRRHR(D3dShader::CreateVertexBufferFrom(&m_dbgVertices[0], m_dbgVertices.size(), m_pDbgVertexBuffer));
    CHRRHR(D3dShader::CreateIndexBufferFrom(&m_dbgIndices[0], m_dbgIndices.size(), m_pDbgIndexBuffer));
    CHRRHR(DXUTGetD3D11Device()->CreateRasterizerState(&m_rasterizeDesc, &m_pRasterizeState))

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
    UINT stride = sizeof(D3D11Vertex_PositionColored);
    UINT offset = 0;

    _ASSERTE(m_pRasterizeState);
    DXUTGetD3D11DeviceContext()->RSSetState(m_pRasterizeState);

    // Draw Mesh
    //
    DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _ASSERTE(m_pVertexBuffer);
    DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    _ASSERTE(m_pIndexBuffer);
    DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    DXUTGetD3D11DeviceContext()->DrawIndexed((UINT)IndexCount(), 0, 0);

    // Draw Debug Local Coordinate System
    //
    DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    _ASSERTE(m_pDbgVertexBuffer);
    DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_pDbgVertexBuffer, &stride, &offset);
    _ASSERTE(m_pDbgIndexBuffer);
    DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_pDbgIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    DXUTGetD3D11DeviceContext()->DrawIndexed(m_dbgIndices.size(), 0, 0);
}