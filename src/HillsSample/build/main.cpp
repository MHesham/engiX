#include "stdafx.h"
#include "D3dApp.h"
#include "engiX.h"
#include "Logger.h"
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "GeometryGenerator.h"

using namespace DirectX; 
using namespace DirectX::PackedVector;
using namespace std;
using namespace engiX;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class HillsApp : public D3dApp
{
public:
	HillsApp(HINSTANCE hInstance);
	~HillsApp();

	bool VInit();
	void VOnResize();
	void VUpdateScene(const Timer& time);
	void VDrawScene(); 

	void VOnMouseDown(WPARAM btnState, int x, int y);
	void VOnMouseUp(WPARAM btnState, int x, int y);
	void VOnMouseMove(WPARAM btnState, int x, int y);

private:
	real GetHeight(real x, real z)const;
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();

private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;

	ID3D11InputLayout* mInputLayout;

	// Define transformations from local spaces to world space.
	XMFLOAT4X4 mGridWorld;

	UINT mGridIndexCount;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

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

    HillsApp sample(hInstance);

    if(!sample.VInit())
    {
        LogError("Failed to initialize app");
        return 0;
    }

    return sample.Run();
}

HillsApp::HillsApp(HINSTANCE hInstance)
: D3dApp(hInstance), mVB(0), mIB(0), mFX(0), mTech(0),
  mfxWorldViewProj(0), mInputLayout(0), mGridIndexCount(0),
  mTheta(1.5f*MathHelper::Pi), mPhi(0.1f*MathHelper::Pi), mRadius(200.0f)
{
	mMainWndCaption = L"Hills Demo";
	
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}

HillsApp::~HillsApp()
{
	SafeRelease(mVB);
	SafeRelease(mIB);
	SafeRelease(mFX);
	SafeRelease(mInputLayout);
}

bool HillsApp::VInit()
{
	if(!D3dApp::VInit())
		return false;

	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();

	return true;
}

void HillsApp::VOnResize()
{
	D3dApp::VOnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void HillsApp::VUpdateScene(const Timer& time)
{
	// Convert Spherical to Cartesian coordinates.
    XMFLOAT3 cartesianXyz;

    MathHelper::ConvertSphericalToCartesian(mRadius, mTheta, mPhi, cartesianXyz);

	// Build the view matrix.
    XMVECTOR pos    = XMVectorSet(cartesianXyz.x, cartesianXyz.y, cartesianXyz.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
}

void HillsApp::VDrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const real*>(&Colors::LightSteelBlue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(mInputLayout);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

 
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	
	XMMATRIX view  = XMLoadFloat4x4(&mView);
	XMMATRIX proj  = XMLoadFloat4x4(&mProj);
	XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
	XMMATRIX worldViewProj = world*view*proj;
 
    D3DX11_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		// Draw the grid.
		mfxWorldViewProj->SetMatrix(reinterpret_cast<real*>(&worldViewProj));
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, 0, 0);
    }

	CHR(mSwapChain->Present(0, 0));
}

void HillsApp::VOnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void HillsApp::VOnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void HillsApp::VOnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		real dx = XMConvertToRadians(0.25f*static_cast<real>(x - mLastMousePos.x));
		real dy = XMConvertToRadians(0.25f*static_cast<real>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		real dx = 0.2f*static_cast<real>(x - mLastMousePos.x);
		real dy = 0.2f*static_cast<real>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 50.0f, 500.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

real HillsApp::GetHeight(real x, real z)const
{
	return 0.3f*( z*sinf(0.1f*x) + x*cosf(0.1f*z) );
}

void HillsApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;
 
	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	mGridIndexCount = grid.Indices.size();

	//
	// Extract the vertex elements we are interested and apply the height function to
	// each vertex.  In addition, color the vertices based on their height so we have
	// sandy looking beaches, grassy low hills, and snow mountain peaks.
	//

	std::vector<Vertex> vertices(grid.Vertices.size());
	for(size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;

		p.y = GetHeight(p.x, p.z);

		vertices[i].Pos   = p;
		
		// Color the vertex based on its height.
		if( p.y < -10.0f )
		{
			// Sandy beach color.
			vertices[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if( p.y < 5.0f )
		{
			// Light yellow-green.
			vertices[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if( p.y < 12.0f )
		{
			// Dark yellow-green.
			vertices[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if( p.y < 20.0f )
		{
			// Dark brown.
			vertices[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			// White snow.
			vertices[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * grid.Vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    CHR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mGridIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &grid.Indices[0];
    CHR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
 
void HillsApp::BuildFX()
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

	mTech    = mFX->GetTechniqueByName("DefaultTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void HillsApp::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	CHR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayout));
}