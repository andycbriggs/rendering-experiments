#include "EffectTechnique.hpp"

#include "Helpers.hpp"
#include "EffectPass.hpp"

using namespace std;

EffectTechnique::EffectTechnique(ID3DX11EffectTechnique* effectTechnique)
    : mEffectTechnique(effectTechnique)
{
    D3DX11_TECHNIQUE_DESC techniqueDesc = { 0 };
    HRESULT hr = effectTechnique->GetDesc(&techniqueDesc);
    ThrowIfFailed(hr, GetDesc);

    mName = techniqueDesc.Name;

    for (UInt32 i = 0; i < techniqueDesc.Passes; ++i)
    {
        mPasses.emplace_back(make_unique<EffectPass>(effectTechnique->GetPassByIndex(i)));
        mPassesByName.emplace(mPasses.back()->GetName(), mPasses.back().get());
    }
}

EffectTechnique::~EffectTechnique() = default;
