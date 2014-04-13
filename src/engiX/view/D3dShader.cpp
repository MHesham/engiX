#include "D3dShader.h"
#include <fstream>
#include <vector>
#include "engiXDefs.h"
#include "Logger.h"
#include "DXUT.h"
#include "Geometry.h"

using namespace engiX;
using namespace std;

D3dDefaultShader::D3dDefaultShader()
{
    m_pVertexLayout = nullptr;
    m_pFX = nullptr;
    m_pFxTech = nullptr;
    m_pFxWvpMatrix = nullptr;
}

D3dDefaultShader::~D3dDefaultShader()
{
    // It is a good practice to destroy objects in the reverse order
    // of their creation one to avoid dangling pointers/deadlocks
    // and any memory weird behavior that relates to ordering
    SAFE_RELEASE(m_pFX);
    SAFE_RELEASE(m_pVertexLayout);
}

StrongD3dShaderPtr D3dDefaultShader::FromCompiledShaderFile(_In_ const wchar_t* pFxFilename)
{
    HRESULT hr = S_OK;
    ifstream fxFile;

    fxFile.open(pFxFilename, ios::in | ios::binary);

    if (!fxFile.is_open())
    {
        LogError("Failed to open fx file %s", pFxFilename);
        return nullptr;
    }

    fxFile.seekg(0, ios::end);
    size_t fxBinarySize = (size_t)fxFile.tellg();
    fxFile.seekg(0, ios::beg);

    vector<char> fxBinary(fxBinarySize);
    fxFile.read(&fxBinary[0], fxBinarySize);
    fxFile.close();

    shared_ptr<D3dDefaultShader> pTempShader(eNEW D3dDefaultShader);

    CHR(D3DX11CreateEffectFromMemory((const void*)&fxBinary[0], (SIZE_T)fxBinarySize, 
        0, DXUTGetD3D11Device(), &pTempShader->m_pFX));

    if (SUCCEEDED(hr))
    {
        pTempShader->m_pFxTech = pTempShader->m_pFX->GetTechniqueByName("DefaultTech");
        pTempShader->m_pFxWvpMatrix = pTempShader->m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();

        // Create the input layout using the vertex format
        // Pass the shader input signature to get it validated against the vertex format provided
        // to ensure type consistency
        D3DX11_PASS_DESC passDesc;
        CHR(pTempShader->m_pFxTech->GetPassByIndex(0)->GetDesc(&passDesc));

        if (SUCCEEDED(hr))
        {
            CHR(DXUTGetD3D11Device()->CreateInputLayout(D3D11VertexLayout_PositionColored, 2, passDesc.pIAInputSignature, 
                passDesc.IAInputSignatureSize, &pTempShader->m_pVertexLayout));

            if (SUCCEEDED(hr))
            {
                return StrongD3dShaderPtr(pTempShader);
            }
        }
    }

    return nullptr;
}

HRESULT D3dDefaultShader::VApply()
{
    HRESULT hr;

    // Set the vertex shader and the vertex layout
    DXUTGetD3D11DeviceContext()->IASetInputLayout(m_pVertexLayout);

    // For now we use a shader with 1 Tech and 1 Pass
    CHR(m_pFxTech->GetPassByIndex(0)->Apply(0, DXUTGetD3D11DeviceContext()));

    return hr;
}

HRESULT D3dDefaultShader::CreateVertexBufferFrom(_In_ const void* pVertexMemSource, _In_ size_t vertexCount, _Out_ ID3D11Buffer*& pVB) const
{
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(D3D11Vertex_PositionColored) * vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &pVertexMemSource;

    CHRRHR(DXUTGetD3D11Device()->CreateBuffer(&vbd, &vinitData, &pVB));

    return S_OK;
}

HRESULT D3dDefaultShader::CreateIndexBufferFrom(_In_ const void* pIndexMemSource, _In_ size_t indexCount, _Out_ ID3D11Buffer*& pIB) const
{
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &pIndexMemSource;

    CHRRHR(DXUTGetD3D11Device()->CreateBuffer(&ibd, &iinitData, &pIB));

    return S_OK;
}

