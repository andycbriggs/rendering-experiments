#pragma once

struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3DX11EffectPass;

class EffectPass
{
public:
    EffectPass(ID3DX11EffectPass* effectPass);
    ~EffectPass();

    const std::string& GetName() const { return mName; }

    void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementsDesc, UInt32 numElements, ID3D11InputLayout** inputLayout) const;
    void Apply(UINT flags, ID3D11DeviceContext* context) const;

private:
    std::string mName;
    ID3DX11EffectPass* mPass;
};