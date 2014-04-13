#pragma once

#include "engiXDefs.h"
#include <d3d11.h>

namespace engiX
{
    struct D3D11Vertex_PositionColored
    {
        Vec3 Position;
        Vec3 Color;
    };

    const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_PositionColored[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
}