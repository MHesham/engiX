#include "D3dShader.h"
#include <fstream>
#include <vector>
#include "engiXDefs.h"
#include "Logger.h"
#include "DXUT.h"

using namespace engiX;
using namespace std;

D3dVertexShader::D3dVertexShader()
{
    m_pVertexLayout = nullptr;
    m_pVertexShader = nullptr;
    m_pFX = nullptr;
    m_pFxTech = nullptr;
    m_pFxWvpMatrix = nullptr;
}

D3dVertexShader::~D3dVertexShader()
{
    // It is a good practice to destroy objects in the reverse order
    // of their creation one to avoid dangling pointers/deadlocks
    // and any memory weird behavior that relates to ordering
    SAFE_DELETE(m_pFxWvpMatrix); // World View Projection matrix
    SAFE_DELETE(m_pFxTech);
    SAFE_DELETE(m_pFX);
    SAFE_DELETE(m_pVertexShader);
    SAFE_DELETE(m_pVertexLayout);
}

shared_ptr<D3dVertexShader> D3dVertexShader::FromCompiledShaderFile(_In_ const wchar_t* pFxFilename)
{
    HRESULT hr = E_FAIL;
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

    shared_ptr<D3dVertexShader> pTempShader(eNEW D3dVertexShader);

    CHR(D3DX11CreateEffectFromMemory((const void*)&fxBinary[0], (SIZE_T)fxBinarySize, 
        0, DXUTGetD3D11Device(), &pTempShader->m_pFX));

    if (SUCCEEDED(hr))
    {
        pTempShader->m_pFxTech = pTempShader->m_pFX->GetTechniqueByName("DefaultTech");
        pTempShader->m_pFxWvpMatrix = pTempShader->m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();

        // Describe the vertex format
        D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        // Create the input layout using the vertex format
        // Pass the shader input signature to get it validated against the vertex format provided
        // to ensure type consistency
        D3DX11_PASS_DESC passDesc;
        CHR(pTempShader->m_pFxTech->GetPassByIndex(0)->GetDesc(&passDesc));

        if (SUCCEEDED(hr))
        {
            CHR(DXUTGetD3D11Device()->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
                passDesc.IAInputSignatureSize, &pTempShader->m_pVertexLayout));

            if (SUCCEEDED(hr))
            {
                return pTempShader;
            }
        }
    }

    return nullptr;
}

bool D3dVertexShader::Apply()
{
    HRESULT hr;

    // Set the vertex shader and the vertex layout
    DXUTGetD3D11DeviceContext()->IASetInputLayout(m_pVertexLayout);

    // For now we use a shader with 1 Tech and 1 Pass
    CHR(m_pFxTech->GetPassByIndex(0)->Apply(0, DXUTGetD3D11DeviceContext()));

    return (SUCCEEDED(hr) ? true : false);
}
