#include "Material.hpp"

#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"

using namespace std;

Material::Material() = default;
Material::~Material() = default;

void Material::CreateInputLayout(const string& techniqueName, const string& passName, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UInt32 numElements)
{
   const EffectTechnique* technique = mEffect->GetTechnique(techniqueName);
   const EffectPass* pass = technique->GetPass(passName);

   ComPtr<ID3D11InputLayout> inputLayout;
   pass->CreateInputLayout(inputElementDesc, numElements, inputLayout.ReleaseAndGetAddressOf());

   mInputLayouts.emplace(pass, inputLayout);
}

void Material::SetCurrentTechnique(const string& techniqueName)
{
	EffectTechnique* technique = mEffect->GetTechnique(techniqueName);

	if (technique)
		mCurrentTechnique = technique;
}

EffectTechnique* Material::GetCurrentTechnique() const
{
    return mCurrentTechnique != nullptr ? mCurrentTechnique : mEffect->GetTechniques()[0].get(); 
}