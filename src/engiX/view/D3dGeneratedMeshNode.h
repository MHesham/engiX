#pragma once

#include "SceneNode.h"
#include "D3dShader.h"
#include "Geometry.h"

namespace engiX
{
    class D3dGeneratedMeshNode : public SceneNode
    {
    public:
        typedef std::vector<D3D11Vertex_PositionColored> VertexList;
        typedef std::vector<UINT> IndexList;

        D3dGeneratedMeshNode(_In_ ActorID actorId, _In_ const GeometryGenerator::MeshData& mesh, _In_ Color3 color, _In_ GameScene* pScene);
        ~D3dGeneratedMeshNode();

        HRESULT OnPreRender();
        HRESULT OnConstruct();
        void OnRender();
        ID3D11Buffer* VertextBuffer() { return m_pVertexBuffer; }
        ID3D11Buffer* IndexBuffer() { return m_pIndexBuffer; }
        size_t VertexCount() const { return m_vertices.size(); }
        size_t IndexCount() const { return m_indices.size(); }
        bool RenderWireframe() const { return m_rasterizeDesc.FillMode == D3D11_FILL_WIREFRAME; }
        bool RenderBackface() const { return m_rasterizeDesc.CullMode != D3D11_CULL_BACK; }

    protected:
        VertexList m_vertices;
        IndexList m_indices;
        VertexList m_dbgVertices;
        IndexList m_dbgIndices;
        D3dShader m_shader;
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pIndexBuffer;
        ID3D11Buffer* m_pDbgVertexBuffer;
        ID3D11Buffer* m_pDbgIndexBuffer;
        D3D11_RASTERIZER_DESC m_rasterizeDesc;
        ID3D11RasterizerState* m_pRasterizeState;
    };
}