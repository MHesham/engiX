#pragma once
//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines a static class for procedurally generating the geometry of 
// common mathematical objects.
//
// All triangles are generated "outward" facing.  If you want "inward" 
// facing triangles (for example, if you want to place the camera inside
// a sphere to simulate a sky), you will need to:
//   1. Change the Direct3D cull mode or manually reverse the winding order.
//   2. Invert the normal.
//   3. Update the texture coordinates and tangent vectors.
//***************************************************************************************

#include "D3dUtil.h"
#include <DirectXColors.h>

namespace engiX
{
    class GeometryGenerator
    {
    public:
        struct Vertex
        {
            Vertex()
            {
                ZeroMemory(this, sizeof(*this));
            }

            Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv)
                : Position(p), Normal(n), TangentU(t), TexC(uv), Color(DirectX::Colors::Black) {}
            Vertex(
                float px, float py, float pz, 
                float nx, float ny, float nz,
                float tx, float ty, float tz,
                float u, float v)
                : Position(px,py,pz), Normal(nx,ny,nz),
                TangentU(tx, ty, tz), TexC(u,v), Color(DirectX::Colors::Black) {}

            DirectX::XMFLOAT3 Position;
            DirectX::XMFLOAT3 Normal;
            DirectX::XMFLOAT3 TangentU;
            DirectX::XMFLOAT2 TexC;
            DirectX::XMFLOAT3 Color;
        };

        class MeshData
        {
        public:
            MeshData() :
                IsWireframe(true),
                IsBackfacing(true)
            {}

            std::vector<Vertex> Vertices;
            std::vector<UINT> Indices;
            bool IsWireframe;
            bool IsBackfacing;
        };

        ///<summary>
        /// Creates a box centered at the origin with the given dimensions.
        ///</summary>
        void CreateBox(_In_ float width, _In_ float height, _In_ float depth, _Out_ MeshData& meshData);

        ///<summary>
        /// Creates a sphere centered at the origin with the given radius.  The
        /// slices and stacks parameters control the degree of tessellation.
        ///</summary>
        void CreateSphere(_In_ float radius, _In_ UINT sliceCount, _In_ UINT stackCount, _Out_ MeshData& meshData);

        ///<summary>
        /// Creates a geosphere centered at the origin with the given radius.  The
        /// depth controls the level of tessellation.
        ///</summary>
        void CreateGeosphere(_In_ float radius, _In_ UINT numSubdivisions, _Out_ MeshData& meshData);

        ///<summary>
        /// Creates a cylinder parallel to the y-axis, and centered about the origin.  
        /// The bottom and top radius can vary to form various cone shapes rather than true
        // cylinders.  The slices and stacks parameters control the degree of tessellation.
        ///</summary>
        void CreateCylinder(_In_ float bottomRadius, _In_ float topRadius, _In_ float height, _In_ UINT sliceCount, _In_ UINT stackCount, _Out_ MeshData& meshData);

        ///<summary>
        /// Creates an mxn grid in the xz-plane with m rows and n columns, centered
        /// at the origin with the specified width and depth.
        ///</summary>
        void CreateGrid(_In_ float width, _In_ float depth, _In_ UINT m, _In_ UINT n, _Out_ MeshData& meshData);

        ///<summary>
        /// Creates a quad covering the screen in NDC coordinates.  This is useful for
        /// postprocessing effects.
        ///</summary>
        void CreateFullscreenQuad(_Out_ MeshData& meshData);

        ///<summary>
        /// Creates 3 perpendicular axises X, Y, Z
        /// X: Right Red Axis
        /// Y: Up Green Axis
        /// Z: Front Blue Axis
        /// The 3 axises are formatted as line list where each consecutive
        /// 2 vertices represent an axis
        ///</summary>
        static void CreateCartesianCoordAxises(_Out_ MeshData& meshData);

    private:
        void Subdivide(MeshData& meshData);
        void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
        void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData& meshData);
    };
}
