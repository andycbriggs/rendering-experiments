#include "TextureCube.hpp"

#include <DDSTextureLoader.h>

#include "Engine.hpp"

using namespace std;
using namespace DirectX;
    
TextureCube::TextureCube(const string& filename)
{
    const wstring wideFilename(filename.begin(), filename.end());

    HRESULT hr = CreateDDSTextureFromFile(Engine::Get()->Device(), wideFilename.c_str(), 
        nullptr, mShaderResourceView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateDDSTextureFromFile);
}