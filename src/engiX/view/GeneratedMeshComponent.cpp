#include "GeneratedMeshComponent.h"

using namespace engiX;

GeneratedMeshComponent::GeneratedMeshComponent(_In_ const GeometryGenerator::MeshData& mesh, _In_ Color3 color) :
    m_pIndexBuffer(nullptr),
    m_pVertexBuffer(nullptr),
    m_color(color)
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
