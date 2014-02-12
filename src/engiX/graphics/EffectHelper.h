#pragma once

#include <string>
#include "d3dx11Effect.h"

namespace engiX
{
    class EffectHelper
    {
    public:
        static HRESULT LoadBinaryEffectFromFile(_In_ const std::string& filename, _In_ ID3D11Device* pDevice, _Out_ ID3DX11Effect*& pFx);
    };
}