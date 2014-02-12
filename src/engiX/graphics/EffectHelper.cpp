#include "EffectHelper.h"
#include <fstream>
#include <vector>
#include "engiXDefs.h"

using namespace engiX;
using namespace std;

HRESULT EffectHelper::LoadBinaryEffectFromFile(_In_ const std::string& filename, _In_ ID3D11Device* pDevice, _Out_ ID3DX11Effect*& pFx)
{
    std::ifstream fin(filename, std::ios::binary);

    CBRE(fin.is_open(), HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));

    fin.seekg(0, std::ios_base::end);
    int size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);

    std::vector<char> compiledShader(size);

    fin.read(&compiledShader[0], size);
    fin.close();

    CHRRE(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
        0, pDevice, &pFx));

    return S_OK;
}
