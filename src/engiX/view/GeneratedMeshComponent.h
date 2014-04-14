#pragma 

#include "Actor.h"
#include "GeometryGenerator.h"
#include <memory>
#include "Geometry.h"
#include "D3dShader.h"

namespace engiX
{
    class GeneratedMeshComponent : public ActorComponent
    {
    public:
        typedef std::vector<D3D11Vertex_PositionColored> VertexList;
        typedef std::vector<UINT> IndexList;

        GeneratedMeshComponent(_In_ GeometryGenerator::MeshData mesh, _In_ Color3 color);
        ~GeneratedMeshComponent();
        static const ComponentID TypeID = 0x6211EC48;
        ComponentID TypeId() const { return TypeID; }
        const wchar_t* Typename() const { return L"GeneratedMeshComponent"; }
        void OnUpdate(_In_ const Timer& time) {}
        bool Init();
        
    protected:
        StrongD3dShaderPtr m_pShader;
        VertexList m_vertices;
        IndexList m_indices;
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pIndexBuffer;
    };
};