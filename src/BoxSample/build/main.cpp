#include "stdafx.h"
#include "D3dApp.h"
#include "engiX.h"
#include "Logger.h"
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>


using namespace DirectX; 
using namespace DirectX::PackedVector;
using namespace std;
using namespace engiX;


struct Vertex
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
};

class BoxApp : public D3dApp
{
public:
    BoxApp(HINSTANCE hInstance);
    ~BoxApp();
    bool VInit();

protected:
    void VOnResize();
    void VUpdateScene(const Timer& timer);
    void VDrawScene(); 

    void VOnMouseDown(WPARAM btnState, int x, int y);
    void VOnMouseUp(WPARAM btnState, int x, int y);
    void VOnMouseMove(WPARAM btnState, int x, int y);

private:
    void BuildGeometryBuffers();
    void BuildFX();
    void BuildVertexLayout();

    ID3D11Buffer* mBoxVB;
    ID3D11Buffer* mBoxIB;

    ID3DX11Effect* mFX;
    ID3DX11EffectTechnique* mTech;
    ID3DX11EffectMatrixVariable* mfxWorldViewProj;

    ID3D11InputLayout* mInputLayout;

    DirectX::XMFLOAT4X4 mWorld;
    DirectX::XMFLOAT4X4 mView;
    DirectX::XMFLOAT4X4 mProj;

    real mTheta;
    real mPhi;
    real mRadius;

    POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
                   PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    if (!engiX::Init())
    {
        wprintf_s(L"Failed to initialize one or all of engiX subsystems");
        return 0;
    }

    LogInfo("engiX subsystems initialized successfully");

    LogInfo("Starting SolarSample");

    BoxApp sample(hInstance);

    if(!sample.VInit())
    {
        LogError("Failed to initialize app");
        return 0;
    }

    return sample.Run();
}

BoxApp::BoxApp(HINSTANCE hInstance)
    : D3dApp(hInstance), mBoxVB(0), mBoxIB(0), mFX(0), mTech(0),
    mfxWorldViewProj(0), mInputLayout(0), 
    mTheta(1.5f*MathHelper::Pi), mPhi(0.25f*MathHelper::Pi), mRadius(5.0f)
{
    mMainWndCaption = L"Box Demo";

    mLastMousePos.x = 0;
    mLastMousePos.y = 0;

    DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();
    DirectX::XMStoreFloat4x4(&mWorld, I);
    DirectX::XMStoreFloat4x4(&mView, I);
    DirectX::XMStoreFloat4x4(&mProj, I);
}

BoxApp::~BoxApp()
{
    SafeRelease(mBoxVB);
    SafeRelease(mBoxIB);
    SafeRelease(mFX);
    SafeRelease(mInputLayout);
}

bool BoxApp::VInit()
{
    if(!D3dApp::VInit())
        return false;

    BuildGeometryBuffers();
    BuildFX();
    BuildVertexLayout();

    return true;
}

void BoxApp::BuildGeometryBuffers()
{
    // Create vertex buffer
    Vertex vertices[] =
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White)   },
        { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black)   },
        { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red)     },
        { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green)   },
        { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue)    },
        { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow)  },
        { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan)    },
        { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }
    };

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * 8;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    CHR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));


    // Create the index buffer

    UINT indices[] = {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3, 
        4, 3, 7
    };

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * 36;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    CHR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
}

void BoxApp::BuildFX()
{
    std::string fxFilename = "data\\fx\\default.fxo";
    ifstream fxFile;

    fxFile.open(fxFilename.c_str(), ios::in | ios::binary);

    if (!fxFile.is_open())
    {
        LogError("Failed to open fx file %s", fxFilename.c_str());
        return;
    }

    fxFile.seekg(0, ios::end);
    size_t fxBinarySize = (size_t)fxFile.tellg();
    fxFile.seekg(0, ios::beg);

    vector<char> fxBinary(fxBinarySize);
    fxFile.read(&fxBinary[0], fxBinarySize);
    fxFile.close();

    CHR(D3DX11CreateEffectFromMemory((const void*)&fxBinary[0], (SIZE_T)fxBinarySize, 
        0, md3dDevice, &mFX));

    mTech = mFX->GetTechniqueByName("DefaultTech");
    mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void BoxApp::BuildVertexLayout()
{
    // Describe the vertex format
    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    // Create the input layout using the vertex format
    // Pass the shader inpute signature to get it validated against the vertex format provided
    // to ensure type consistentcy
    D3DX11_PASS_DESC passDesc;
    CHR(mTech->GetPassByIndex(0)->GetDesc(&passDesc));
    CHR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
        passDesc.IAInputSignatureSize, &mInputLayout));
}


void BoxApp::VOnResize()
{
    D3dApp::VOnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    DirectX::XMStoreFloat4x4(&mProj, proj);
}

void BoxApp::VUpdateScene(const Timer& timer)
{
    // Convert Spherical to Cartesian coordinates.
    real x = mRadius*sinf(mPhi)*cosf(mTheta);
    real z = mRadius*sinf(mPhi)*sinf(mTheta);
    real y = mRadius*cosf(mPhi);

    // Build the view matrix.
    DirectX::XMVECTOR pos    = DirectX::XMVectorSet(x, y, z, 1.0f);
    DirectX::XMVECTOR target = DirectX::XMVectorZero();
    DirectX::XMVECTOR up     = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
    DirectX::XMStoreFloat4x4(&mView, V);
}

void BoxApp::VDrawScene()
{
    md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const real*>(&Colors::LightSteelBlue));
    md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

    md3dImmediateContext->IASetInputLayout(mInputLayout);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
    md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

    // Set constants
    DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorld);
    DirectX::XMMATRIX view  = DirectX::XMLoadFloat4x4(&mView);
    DirectX::XMMATRIX proj  = DirectX::XMLoadFloat4x4(&mProj);
    DirectX::XMMATRIX worldViewProj = world * view * proj;

    CHR(mfxWorldViewProj->SetMatrix(reinterpret_cast<real*>(&worldViewProj.r)));

    D3DX11_TECHNIQUE_DESC techDesc;
    CHR(mTech->GetDesc( &techDesc ));
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        CHR(mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext));

        // 36 indices for the box.
        md3dImmediateContext->DrawIndexed(36, 0, 0);
    }

    CHR(mSwapChain->Present(0, 0));
}

void BoxApp::VOnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}

void BoxApp::VOnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void BoxApp::VOnMouseMove(WPARAM btnState, int x, int y)
{
    if( (btnState & MK_LBUTTON) != 0 )
    {
        // Make each pixel correspond to a quarter of a degree.
        real dx = DirectX::XMConvertToRadians(0.25f*static_cast<real>(x - mLastMousePos.x));
        real dy = DirectX::XMConvertToRadians(0.25f*static_cast<real>(y - mLastMousePos.y));

        // Update angles based on input to orbit camera around box.
        mTheta += dx;
        mPhi   += dy;

        // Restrict the angle mPhi.
        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
    }
    else if( (btnState & MK_RBUTTON) != 0 )
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        real dx = 0.005f*static_cast<real>(x - mLastMousePos.x);
        real dy = 0.005f*static_cast<real>(y - mLastMousePos.y);

        // Update the camera radius based on input.
        mRadius += dx - dy;

        // Restrict the radius.
        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}
