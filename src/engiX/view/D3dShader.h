#pragma once

#include <d3d11.h>
#include <d3dx11effect.h>
#include <memory>
#include "engiXDefs.h"

namespace engiX
{
    class D3dVertexShader
    {
    public:
        D3dVertexShader();
        ~D3dVertexShader();
        bool Apply();


    protected:
        DISALLOW_COPY_AND_ASSIGN(D3dVertexShader);

        ID3D11InputLayout* m_pVertexLayout;
        ID3D11VertexShader* m_pVertexShader;
        ID3DX11Effect* m_pFX;
        ID3DX11EffectTechnique* m_pFxTech;
        ID3DX11EffectMatrixVariable* m_pFxWvpMatrix; // World View Projection matrix

       static std::shared_ptr<D3dVertexShader> FromCompiledShaderFile(_In_ const wchar_t* pFxFilename);
    };
}