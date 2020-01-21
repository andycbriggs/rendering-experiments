#include "BloomComponent.hpp"

#include "BloomMaterial.hpp"
#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "Engine.hpp"
#include "RenderTarget.hpp"

using namespace std;

void BloomComponent::OnInitialise()
{
    BlurComponent::OnInitialise();

    mBloomMaterial = make_unique<BloomMaterial>();
    mBloomMaterial->Initialise();

    ReCreateBloomRenderTarget();
}

void BloomComponent::OnWindowSizeChanged()
{
    BlurComponent::OnWindowSizeChanged();

    ReCreateBloomRenderTarget();
}

void BloomComponent::OnDrawPostProcessing(const RenderState& renderState, const Texture2D* inputTexture)
{
    mBlurRenderTarget->Begin(renderState);
    BlurComponent::OnDrawPostProcessing(renderState, inputTexture);
    mBlurRenderTarget->End();

    mGlowRenderTarget->Begin(renderState);
    mBloomMaterial->SetCurrentTechnique("GlowExtract"); // TODO: render scene to emissive buffer
    DrawCurrentBloomTechnique(renderState, mBlurRenderTarget->GetTexture().get());
    mGlowRenderTarget->End();

    mBloomMaterial->SetCurrentTechnique("Bloom");
    mBloomMaterial->SetSceneTexture(inputTexture); // the original scene texture
    DrawCurrentBloomTechnique(renderState, mGlowRenderTarget->GetTexture().get());
}

void BloomComponent::DrawCurrentBloomTechnique(const RenderState& renderState, const Texture2D* inputTexture)
{
    renderState.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    const EffectPass* pass = mBloomMaterial->GetCurrentTechnique()->GetPasses()[0].get();
    ID3D11InputLayout* inputLayout = mBloomMaterial->GetInputLayout(pass);
    renderState.deviceContext->IASetInputLayout(inputLayout);

    mBloomMaterial->SetInputTexture(inputTexture);

    UInt32 stride = mBloomMaterial->VertexSize();
    UInt32 offset = mBloomMaterial->Offset();
    renderState.deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
    renderState.deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    pass->Apply(0, renderState.deviceContext);
    renderState.deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void BloomComponent::ReCreateBloomRenderTarget()
{
    mBlurRenderTarget = make_unique<RenderTarget>();
    mGlowRenderTarget = make_unique<RenderTarget>();
    // TODO: dont hardcode to Engine
    mBlurRenderTarget->Initialise(UInt32(Engine::Get()->GetViewport().width), UInt32(Engine::Get()->GetViewport().height));
    mGlowRenderTarget->Initialise(UInt32(Engine::Get()->GetViewport().width), UInt32(Engine::Get()->GetViewport().height));
}
