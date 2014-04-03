#pragma once

#include "Precision.h"
#include "Logger.h"

#if defined(DEBUG) | defined(_DEBUG)

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

#ifndef CHRRE
#define CHRRE(x)                                               \
    {                                                       \
    HRESULT hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x, hr);                \
    return hr;                                        \
    }                                                       \
    }
#else
#define CHRRE(x) (x)
#endif

#ifndef CBRE
#define CBRE(x, hr)                                               \
    {                                                       \
    if(!(x))                                         \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x, hr);                \
    return hr;                                        \
    }                                                       \
    }
#else
#define CBRE(x) (x)
#endif

#define DISALLOW_COPY_AND_ASSIGN(C) \
    C(const C&); C& operator = (const C&);

#if defined(_DEBUG)
#	define eNEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define eNEW new
#endif

#endif 