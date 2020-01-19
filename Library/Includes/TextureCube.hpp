#pragma once

#include "Helpers.hpp"

class TextureCube
{
public:
    TextureCube(const std::string& filename);

    ID3D11ShaderResourceView* GetShaderResourceView() const { return mShaderResourceView.Get(); }

private:
    ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
};