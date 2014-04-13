#pragma once

#include "Precision.h"
#include "Logger.h"

#include <DirectXMath.h>

#if defined(DEBUG) | defined(_DEBUG)

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
    LogError("'%s' failed, hr=%x", L#x, hr);                \
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


#define DISALLOW_COPY_AND_ASSIGN(C) \
    C(const C&); C& operator = (const C&);

#if defined(_DEBUG)
#	define eNEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define eNEW new
#endif

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

#endif 