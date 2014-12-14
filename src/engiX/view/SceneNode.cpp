#include "SceneNode.h"
#include <memory>
#include <algorithm>
#include "GameScene.h"
#include "WinGameApp.h"
#include "TransformCmpt.h"
#include "GameLogic.h"
#include "D3dShader.h"
#include "GeometryGenerator.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

SceneNode::SceneNode(_In_ ActorID actorId, _In_ GameScene* pScene) :
m_pScene(pScene),
m_pParent(nullptr),
m_actorId(actorId)
{
    XMStoreFloat4x4(&m_worldTsfm, XMMatrixIdentity());
}

HRESULT SceneNode::OnPreRender()
{
    m_pScene->PushTransformation(m_worldTsfm);
    return S_OK;
}

void SceneNode::OnPostRender()
{
    m_pScene->PopTransformation();
}

void SceneNode::RenderChildren()
{
    for (auto pChild : m_children)
    {
        if (SUCCEEDED(pChild->OnPreRender()))
        {
            pChild->OnRender();
            pChild->OnPostRender();
        }

        pChild->RenderChildren();
    }
}

void SceneNode::OnUpdate(_In_ const Timer& time)
{
    auto& a = g_pApp->Logic()->GetActor(m_actorId);

    if (!a.IsNull())
    {
        m_worldTsfm = a.Get<TransformCmpt>().Transform();
    }

    for (auto pChild : m_children)
        pChild->OnUpdate(time);
}

HRESULT SceneNode::OnConstruct()
{
    for (auto pChild : m_children)
        CHRRHR(pChild->OnConstruct());

    return S_OK;
}

bool SceneNode::AddChild(_In_ shared_ptr<ISceneNode> pChild)
{
    if (m_children.insert(static_pointer_cast<SceneNode>(pChild)).second)
    {
        pChild->Parent(this);
        return true;
    }

    return false;
}

bool SceneNode::RemoveChild(_In_ ActorID actor)
{
    auto where = find_if(m_children.begin(), m_children.end(),
        [actor](const shared_ptr<SceneNode>& child){ return child->m_actorId == actor; });

    if (where == m_children.end())
        return false;

    m_children.erase(where);
    return true;
}

RootSceneNode::RootSceneNode(GameScene* pScene) :
SceneNode(NullActorID, pScene),
m_pDbgCoordsVertexBuffer(nullptr),
m_pDbgCoordsIndexBuffer(nullptr)
{
    GeometryGenerator::MeshData coordsMesh;
    GeometryGenerator::CreateCartesianCoordAxises(coordsMesh);

    m_dbgCoordsVertices.resize(coordsMesh.Vertices.size());

    for (size_t i = 0; i < m_dbgCoordsVertices.size(); ++i)
    {
        m_dbgCoordsVertices[i].Position = coordsMesh.Vertices[i].Position;
        m_dbgCoordsVertices[i].Color = coordsMesh.Vertices[i].Color;
    }

    m_dbgCoordIndices.assign(coordsMesh.Indices.begin(), coordsMesh.Indices.end());
}


RootSceneNode::~RootSceneNode()
{
    SAFE_RELEASE(m_pDbgCoordsVertexBuffer);
    SAFE_RELEASE(m_pDbgCoordsIndexBuffer);
}

HRESULT RootSceneNode::OnConstruct()
{
    SAFE_RELEASE(m_pDbgCoordsVertexBuffer);
    SAFE_RELEASE(m_pDbgCoordsIndexBuffer);

    CHRRHR(D3dShader::CreateVertexBufferFrom(&m_dbgCoordsVertices[0], m_dbgCoordsVertices.size(), m_pDbgCoordsVertexBuffer));
    CHRRHR(D3dShader::CreateIndexBufferFrom(&m_dbgCoordIndices[0], m_dbgCoordIndices.size(), m_pDbgCoordsIndexBuffer));

    return S_OK;
}

void RootSceneNode::RenderCoordAxises()
{
    UINT stride = sizeof(D3D11Vertex_PositionColored);
    UINT offset = 0;

    // Draw Debug Local Coordinate System
    //
    DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    _ASSERTE(m_pDbgCoordsVertexBuffer);
    DXUTGetD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_pDbgCoordsVertexBuffer, &stride, &offset);
    _ASSERTE(m_pDbgCoordsIndexBuffer);
    DXUTGetD3D11DeviceContext()->IASetIndexBuffer(m_pDbgCoordsIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    DXUTGetD3D11DeviceContext()->DrawIndexed(m_dbgCoordIndices.size(), 0, 0);
}