#pragma once

#include "Precision.h"
#include "Logger.h"

#include <DirectXMath.h>
#include <dxerr.h>

#ifndef CHR
#define CHR(x)                                               \
    {                                                       \
    hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x, hr);                \
    }                                                       \
    }
#else
#define CHR(x) (x)
#endif

#ifndef CHRR
#define CHRR(x)                                               \
    {                                                       \
    HRESULT hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x, hr);                \
    return;                                        \
    }                                                       \
    }
#else
#define CHRR(x) (x)
#endif

#ifndef CHRRHR
#define CHRRHR(x)                                               \
    {                                                       \
    HRESULT hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x, hr);                \
    return hr;                                        \
    }                                                       \
    }
#else
#define CHRRHR(x) (x)
#endif

#ifndef CHRRB
#define CHRRB(x)                                               \
    {                                                       \
    HRESULT hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
	LogError("'%s' failed, hr=%x, error=%d, %s", L#x, hr, HRESULT_CODE(hr), DXGetErrorString(hr));                \
	DXTRACE_ERR_MSGBOX(L#x, hr); \
    return (SUCCEEDED(hr) ? true : false);                                        \
    }                                                       \
    }
#else
#define CHRRHR(x) (x)
#endif

#ifndef CBRHR
#define CBRHR(x, hr)                                               \
    {                                                       \
    if(!(x))                                         \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x, hr);                \
    return hr;                                        \
    }                                                       \
    }
#else
#define CBRHR(x) (x)
#endif

#ifndef CBRB
#define CBRB(x)                                               \
    {                                                       \
    if(!(x))                                         \
    {                                                       \
    LogError("'%s' failed", L#x);                \
    return false;                                        \
    }                                                       \
    }
#else
#define CBRB(x) (x)
#endif

#ifndef CBR
#define CBR(x)                                               \
    {                                                       \
    if(!(x))                                         \
    {                                                       \
    LogError("'%s' failed", L#x);                \
    return;                                        \
    }                                                       \
    }
#else
#define CBR(x) (x)
#endif

#define DISALLOW_COPY_AND_ASSIGN(C) \
    C(const C&); C& operator = (const C&);

#define eNEW new

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

typedef DirectX::XMFLOAT4X4 Mat4x4;
typedef DirectX::XMFLOAT3 Vec3;
typedef DirectX::XMFLOAT4 Vec4;
typedef DirectX::XMFLOAT2 Vec2;
typedef DirectX::XMFLOAT3 Color3;
