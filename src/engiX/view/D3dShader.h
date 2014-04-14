#pragma once

#include <d3d11.h>
#include <d3dx11effect.h>
#include <memory>
#include "engiXDefs.h"
#include "GeometryGenerator.h"
#include "ViewInterfaces.h"
#include <string>

namespace engiX
{
    typedef std::shared_ptr<ID3dShader> StrongD3dShaderPtr;

    class D3dShader : public ID3dShader
    {
    public:
        D3dShader(_In_ const wchar_t* pFxFilename);
        ~D3dShader();
        HRESULT OnPreRender(ISceneNode* pNode);
        HRESULT OnConstruct();

        static HRESULT CreateVertexBufferFrom(_In_ const void* pVertexMemSource, _In_ size_t vertexCount, _Out_ ID3D11Buffer*& pVB);
        static HRESULT CreateIndexBufferFrom(_In_ const void* pIndexMemSource, _In_ size_t indexCount, _Out_ ID3D11Buffer*& pIB);

    protected:
        DISALLOW_COPY_AND_ASSIGN(D3dShader);

        ID3D11InputLayout* m_pVertexLayout;
        ID3DX11Effect* m_pFX;
        ID3DX11EffectTechnique* m_pFxTech;
        ID3DX11EffectMatrixVariable* m_pFxWvpMatrix; // World View Projection matrix
        std::wstring m_fxFilename;
    };
}