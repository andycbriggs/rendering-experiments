#pragma once

#include "Helpers.hpp"

class Texture2D
{
public:
    enum Usage
    {
        Bitmap,
        Stencil
    };

    Texture2D(const std::string& filename);
    Texture2D(UInt32 width, UInt32 height, Usage usage);

    ID3D11ShaderResourceView* GetShaderResourceView() const { return mShaderResourceView.Get(); }
    ID3D11Texture2D* GetTexture2D() const { return mTexture2D.Get(); }
    ID3D11Resource* GetResource() const { return mResource.Get(); }

	UInt32 GetWidth() const { return mWidth; }
	UInt32 GetHeight() const { return mHeight; }

private:
	UInt32 mWidth = -1;
	UInt32 mHeight = -1;

    ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
    ComPtr<ID3D11Texture2D> mTexture2D;
    ComPtr<ID3D11Resource> mResource;
};