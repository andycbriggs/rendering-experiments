#include "Effect.hpp"

#include "Helpers.hpp"
#include "Engine.hpp"
#include "EffectTechnique.hpp"
#include "EffectVariable.hpp"

using namespace std;

Effect::Effect(const wstring& filename)
{
    HRESULT hr = D3DX11CreateEffectFromFile(filename.c_str(), NULL, Engine::Get()->Device(), mEffect.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, D3DX11CreateEffectFromFile);

    D3DX11_EFFECT_DESC effectDesc;
    hr = mEffect->GetDesc(&effectDesc);
    ThrowIfFailed(hr, GetDesc);

    for (UInt32 i = 0; i < effectDesc.Techniques; ++i)
    {
        mTechniques.emplace_back(make_unique<EffectTechnique>(mEffect->GetTechniqueByIndex(i)));
        mTechniquesByName.emplace(mTechniques.back()->GetName(), mTechniques.back().get());
    }

    for (UInt32 i = 0; i < effectDesc.GlobalVariables; ++i)
    {
        mVariables.emplace_back(make_unique<EffectVariable>(mEffect->GetVariableByIndex(i)));
        mVariablesByName.emplace(mVariables.back()->GetName(), mVariables.back().get());
    }
}

Effect::~Effect() = default;