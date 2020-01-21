#include "BloomMaterial.hpp"

#include <VertexTypes.h>

#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "EffectVariable.hpp"
#include "Texture2D.hpp"

using namespace std;

void BloomMaterial::Initialise()
{
    mEffect = make_unique<Effect>(L"Content\\Effects\\Bloom.cso");

    // Create input layouts for all techniques
    // TODO: Why not just do this for everything?
    for (auto& technique : mEffect->GetTechniques())
    {
        for (auto& pass : technique->GetPasses())
        {
            CreateInputLayout(technique->GetName(), pass->GetName(), VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount);
        }
    }
}

void BloomMaterial::SetSceneTexture(const Texture2D* texture)
{
    EffectVariable* variable = mEffect->GetVariable("SceneTexture");
    variable->SetValue(texture->GetShaderResourceView());
}
