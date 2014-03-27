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

#endif 