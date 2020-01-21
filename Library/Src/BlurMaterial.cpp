#include "BlurMaterial.hpp"

#include <VertexTypes.h>

#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "EffectVariable.hpp"

using namespace std;

void BlurMaterial::Initialise()
{
    mEffect = make_unique<Effect>(L"Content\\Effects\\Blur.cso");

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

void BlurMaterial::SetSampleOffsets(const std::vector<Vector2>& offsets)
{
    EffectVariable* variable = mEffect->GetVariable("SampleOffsets");
    variable->SetValue(offsets);
}

void BlurMaterial::SetSampleWeights(const std::vector<float>& weights)
{
    EffectVariable* variable = mEffect->GetVariable("SampleWeights");
    variable->SetValue(weights);
}
