#include "D3dShader.h"
#include <fstream>
#include <vector>
#include "engiXDefs.h"
#include "Logger.h"
#include "DXUT.h"
#include "Geometry.h"
#include "GameScene.h"

using namespace engiX;
using namespace std;
using namespace DirectX;

D3dShader::D3dShader(_In_ const wchar_t* pFxFilename)
{
    m_pVertexLayout = nullptr;
    m_pFX = nullptr;
    m_pFxTech = nullptr;
    m_pFxWvpMatrix = nullptr;

    m_fxFilename = pFxFilename;
}

D3dShader::~D3dShader()
{
    // It is a good practice to destroy objects in the reverse order
    // of their creation one to avoid dangling pointers/deadlocks
    // and any memory weird behavior that relates to ordering
    SAFE_RELEASE(m_pFX);
    SAFE_RELEASE(m_pVertexLayout);
}

HRESULT D3dShader::OnConstruct()
{
    SAFE_RELEASE(m_pFX);
    SAFE_RELEASE(m_pVertexLayout);

    ifstream fxFile;

    fxFile.open(m_fxFilename, ios::in | ios::binary);

    if (!fxFile.is_open())
    {
        LogError("Failed to open fx file %s", m_fxFilename.c_str());
        return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
    }

    fxFile.seekg(0, ios::end);
    size_t fxBinarySize = (size_t)fxFile.tellg();
    fxFile.seekg(0, ios::beg);

    vector<char> fxBinary(fxBinarySize);
    fxFile.read(&fxBinary[0], fxBinarySize);
    fxFile.close();

    CHRRHR(D3DX11CreateEffectFromMemory((const void*)&fxBinary[0], (SIZE_T)fxBinarySize, 
        0, DXUTGetD3D11Device(), &m_pFX));

    m_pFxTech = m_pFX->GetTechniqueByName("DefaultTech");
    m_pFxWvpMatrix = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();

    // Create the input layout using the vertex format
    // Pass the shader input signature to get it validated against the vertex format provided
    // to ensure type consistency
    D3DX11_PASS_DESC passDesc;
    CHRRHR(m_pFxTech->GetPassByIndex(0)->GetDesc(&passDesc));

    CHRRHR(DXUTGetD3D11Device()->CreateInputLayout(D3D11VertexLayout_PositionColored, 2, passDesc.pIAInputSignature, 
        passDesc.IAInputSignatureSize, &m_pVertexLayout));

    return S_OK;
}

HRESULT D3dShader::OnPreRender(ISceneNode* pNode)
{
    // Set the vertex shader and the vertex layout
    DXUTGetD3D11DeviceContext()->IASetInputLayout(m_pVertexLayout);

    _ASSERTE(pNode);
    _ASSERTE(pNode->Scene());
    _ASSERTE(pNode->Scene()->Camera());
    Mat4x4 wvpMat = pNode->Scene()->CameraWorldViewProjMatrix();

    XMMATRIX wvpXMat;
    wvpXMat = XMLoadFloat4x4(&wvpMat);

    if (m_pFxWvpMatrix && m_pFxTech)
    {
        m_pFxWvpMatrix->SetMatrix(reinterpret_cast<float*>(&wvpXMat));

        // For now we use a shader with 1 Tech and 1 Pass
        _ASSERTE(m_pFxTech);
        CHRRHR(m_pFxTech->GetPassByIndex(0)->Apply(0, DXUTGetD3D11DeviceContext()));
    }

    return S_OK;
}

HRESULT D3dShader::CreateVertexBufferFrom(_In_ void* pVertexMemSource, _In_ size_t vertexCount, _Out_ ID3D11Buffer*& pVB)
{
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(D3D11Vertex_PositionColored) * vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = pVertexMemSource;

    CHRRHR(DXUTGetD3D11Device()->CreateBuffer(&vbd, &vinitData, &pVB));
    const char vbName[] = "D3dShaderVertexBuffer";
    pVB->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(vbName), vbName);

    return S_OK;
}

HRESULT D3dShader::CreateIndexBufferFrom(_In_ void* pIndexMemSource, _In_ size_t indexCount, _Out_ ID3D11Buffer*& pIB)
{
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = pIndexMemSource;

    CHRRHR(DXUTGetD3D11Device()->CreateBuffer(&ibd, &iinitData, &pIB));
    const char ibName[] = "D3dShaderIndexBuffer";
    pIB->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(ibName), ibName);

    return S_OK;
}

