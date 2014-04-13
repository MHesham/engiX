#pragma once

#include <d3d11.h>
#include <d3dx11effect.h>
#include <memory>
#include "engiXDefs.h"
#include "GeometryGenerator.h"

namespace engiX
{
    class ID3dShader
    {
    public:
        virtual ~ID3dShader() {}
        virtual HRESULT VApply() = 0;
        virtual HRESULT CreateVertexBufferFrom(_In_ const void* pVertexMemSource, _In_ size_t vertexCount, _Out_ ID3D11Buffer*& pVB) const = 0;
        virtual HRESULT CreateIndexBufferFrom(_In_ const void* pIndexMemSource, _In_ size_t indexCount, _Out_ ID3D11Buffer*& pIB) const = 0;
    };

    typedef std::shared_ptr<ID3dShader> StrongD3dShaderPtr;

    class D3dDefaultShader : public ID3dShader
    {
    public:
        D3dDefaultShader();
        ~D3dDefaultShader();
        HRESULT VApply();
        HRESULT CreateVertexBufferFrom(_In_ const void* pVertexMemSource, _In_ size_t vertexCount, _Out_ ID3D11Buffer*& pVB) const;
        HRESULT CreateIndexBufferFrom(_In_ const void* pIndexMemSource, _In_ size_t indexCount, _Out_ ID3D11Buffer*& pIB) const;
        static StrongD3dShaderPtr FromCompiledShaderFile(_In_ const wchar_t* pFxFilename);

    protected:
        DISALLOW_COPY_AND_ASSIGN(D3dDefaultShader);

        ID3D11InputLayout* m_pVertexLayout;
        ID3DX11Effect* m_pFX;
        ID3DX11EffectTechnique* m_pFxTech;
        ID3DX11EffectMatrixVariable* m_pFxWvpMatrix; // World View Projection matrix
    };
}