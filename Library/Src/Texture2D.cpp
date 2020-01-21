#include "Texture2D.hpp"

#include <WICTextureLoader.h>

#include "Engine.hpp"

using namespace std;

namespace
{
    // Required for WIC initialisation
    class OneTimeComInitialise
    {
    public:
        OneTimeComInitialise() { 
            CoInitializeEx(nullptr, COINITBASE_MULTITHREADED); 
        }
    };
    static const OneTimeComInitialise oneTimeComInitialise;
};
    
Texture2D::Texture2D(const string& filename)
{
    const wstring wideFilename(filename.begin(), filename.end());

    HRESULT hr = CreateWICTextureFromFile(Engine::Get()->Device(), wideFilename.c_str(), mResource.ReleaseAndGetAddressOf(), mShaderResourceView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateWICTextureFromFile);

    hr = mResource.As(&mTexture2D);
    ThrowIfFailed(hr, As);

    D3D11_TEXTURE2D_DESC texture2DDesc = { 0 };
    mTexture2D->GetDesc(&texture2DDesc);

    mWidth = texture2DDesc.Width;
    mHeight = texture2DDesc.Height;

    SetDebugObjectName(mResource.Get(), "Texture2D");
}

Texture2D::Texture2D(UInt32 width, UInt32 height, Usage usage)
{
    D3D11_TEXTURE2D_DESC texture2DDesc = { 0 };
    texture2DDesc.ArraySize = 1;    
    texture2DDesc.Height = height;
    texture2DDesc.SampleDesc.Count = 1;
    texture2DDesc.SampleDesc.Quality = 0;
    texture2DDesc.Width = width;
    texture2DDesc.MipLevels = 1; // Disable Mip-Mapping

    mWidth = width;
    mHeight = height;

    switch (usage)
    {
        case Bitmap:
            texture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            texture2DDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            break;
        case Stencil:
            texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            texture2DDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
            break;
    }

    HRESULT hr = Engine::Get()->Device()->CreateTexture2D(&texture2DDesc, nullptr, mTexture2D.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, CreateTexture2D);

    if (usage == Bitmap)
    {
        hr = Engine::Get()->Device()->CreateShaderResourceView(mTexture2D.Get(), nullptr, mShaderResourceView.ReleaseAndGetAddressOf());
        ThrowIfFailed(hr, CreateShaderResourceView);
    }
    else
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = { };
        shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = -1;
        hr = Engine::Get()->Device()->CreateShaderResourceView(mTexture2D.Get(), &shaderResourceViewDesc, mShaderResourceView.ReleaseAndGetAddressOf());
        ThrowIfFailed(hr, CreateShaderResourceView);
    }

    hr = mTexture2D.As(&mResource);
    ThrowIfFailed(hr, As);
}