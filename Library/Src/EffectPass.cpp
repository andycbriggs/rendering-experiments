#include "EffectPass.hpp"

#include "Helpers.hpp"
#include "Engine.hpp"

EffectPass::EffectPass(ID3DX11EffectPass* effectPass)
    : mPass(effectPass)
{
    D3DX11_PASS_DESC passDesc = { 0 };
    HRESULT hr = mPass->GetDesc(&passDesc);
    ThrowIfFailed(hr, GetDesc);

    mName = passDesc.Name;
}

EffectPass::~EffectPass() = default;

void EffectPass::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementsDesc, UInt32 numElements, ID3D11InputLayout** inputLayout) const
{
    D3DX11_PASS_DESC passDesc = { 0 };
    HRESULT hr = mPass->GetDesc(&passDesc);
    ThrowIfFailed(hr, GetDesc);
    hr = Engine::Get()->Device()->CreateInputLayout(inputElementsDesc, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, inputLayout);
    ThrowIfFailed(hr, CreateInputLayout);
}

void EffectPass::Apply(UINT flags, ID3D11DeviceContext* context) const
{
    HRESULT hr = mPass->Apply(flags, context);
    ThrowIfFailed(hr, Apply);
}