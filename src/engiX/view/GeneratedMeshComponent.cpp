#include "GeneratedMeshComponent.h"

using namespace engiX;

GeneratedMeshComponent::GeneratedMeshComponent(GeometryGenerator::MeshData mesh, Color3 color) :
    m_pIndexBuffer(nullptr),
    m_pVertexBuffer(nullptr)
{
    m_vertices.resize(mesh.Vertices.size());

    for (size_t i = 0; i < m_vertices.size(); ++i)
    {
        m_vertices[i].Position = mesh.Vertices[i].Position;
        m_vertices[i].Color = color;
    }

    m_indices.assign(mesh.Indices.begin(), mesh.Indices.end());
}

GeneratedMeshComponent::~GeneratedMeshComponent()
{
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertexBuffer);
}

bool GeneratedMeshComponent::Init()
{
    m_pShader = StrongD3dShaderPtr(eNEW D3dShader(L"data/fx/default.fxo"));

    if (m_pShader == nullptr)
    {
        LogError("Failed to load the default shader for %s[%d]", Typename(), TypeId());
        return false;
    }

    //CHRRB(m_pShader->CreateVertexBufferFrom(&m_vertices[0], m_vertices.size(), m_pVertexBuffer));
    //CHRRB(m_pShader->CreateIndexBufferFrom(&m_indices[0], m_indices.size(), m_pIndexBuffer));

    return true;
}
