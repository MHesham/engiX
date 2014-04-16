#pragma once

#include <memory>
#include "Geometry.h"
#include "D3dShader.h"
#include "Actor.h"
#include "GeometryGenerator.h"

namespace engiX
{
    class GeneratedMeshComponent : public ActorComponent
    {
    public:
        typedef std::vector<D3D11Vertex_PositionColored> VertexList;
        typedef std::vector<UINT> IndexList;
        static const ComponentID TypeID = 0x6211EC48;

        GeneratedMeshComponent(_In_ const GeometryGenerator::MeshData& mesh, _In_ Color3 color);
        ~GeneratedMeshComponent();

        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"GeneratedMeshComponent"; }
        Color3 Color() const { return m_color; }
        ID3D11Buffer* VertextBuffer() { return m_pVertexBuffer; }
        ID3D11Buffer* IndexBuffer() { return m_pIndexBuffer; }
        size_t VertexCount() const { return m_vertices.size(); }
        size_t IndexCount() const { return m_indices.size(); }

        void OnUpdate(_In_ const Timer& time) {}
        bool Init() { return true; }

    protected:
        VertexList m_vertices;
        IndexList m_indices;
        Color3 m_color;
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pIndexBuffer;
    };
}