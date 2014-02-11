#pragma once

#include "Logger.h"

#if defined(DEBUG) | defined(_DEBUG)

#ifndef CHE
#define CHE(x)                                               \
    {                                                       \
    HRESULT hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x);                \
    goto exit_error;                                        \
    }                                                       \
    }
#else
#define CHE(x) (x)
#endif

#ifndef CHR
#define CHR(x)                                               \
    {                                                       \
    HRESULT hr = (x);                                       \
    if(FAILED(hr))                                          \
    {                                                       \
    LogError("'%s' failed, hr=%x", L#x);                \
    return;                                        \
    }                                                       \
    }
#else
#define CHR(x) (x)
#endif

#endif 