#include "ColorFilterComponent.hpp"

#include "ColorFilterMaterial.hpp"
#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "RenderState.hpp"
#include "Timer.hpp"

using namespace std;

ColorFilterComponent::ColorFilterComponent() : PostProcessingComponent() {};
ColorFilterComponent::~ColorFilterComponent() {};

void ColorFilterComponent::OnInitialise()
{
    PostProcessingComponent::OnInitialise();

    mMaterial = make_unique<ColorFilterMaterial>();
    mMaterial->Initialise();

    mMaterial->CreateVertexBuffer(mMesh.get(), mVertexBuffer.ReleaseAndGetAddressOf());
}

void ColorFilterComponent::OnDrawPostProcessing(const RenderState& renderState, const Texture2D* inputTexture)
{
    // setup correct material for filter
    switch (mCurrentFilter)
    {
    case Filter::Greyscale:
        mMaterial->SetCurrentTechnique("GreyscaleTechnique");
        break;
    case Filter::Sepia:
        mMaterial->SetCurrentTechnique("SepiaTechnique");
        break;
    case Filter::Invert:
        mMaterial->SetCurrentTechnique("InvertTechnique");
        break;
    case Filter::NoEffect:
    default:
        mMaterial->SetCurrentTechnique("NoEffectTechnique");
        break;
    }

    renderState.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    const EffectPass* pass = mMaterial->GetCurrentTechnique()->GetPasses()[0].get();
    ID3D11InputLayout* inputLayout = mMaterial->GetInputLayout(pass);
    renderState.deviceContext->IASetInputLayout(inputLayout);

    mMaterial->SetInputTexture(inputTexture);

    UInt32 stride = mMaterial->VertexSize();
    UInt32 offset = mMaterial->Offset();
    renderState.deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
    renderState.deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    pass->Apply(0, renderState.deviceContext);
    renderState.deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void ColorFilterComponent::SetFilter(Filter filter)
{
    mCurrentFilter = filter;
}