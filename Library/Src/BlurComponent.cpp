#include "BlurComponent.hpp"

#include "BlurMaterial.hpp"
#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "PipelineStateRestorer.hpp"
#include "RenderState.hpp"
#include "RenderTarget.hpp"
#include "Texture2D.hpp"
#include "Timer.hpp"

using namespace std;

void BlurComponent::OnInitialise()
{
	PostProcessingComponent::OnInitialise();

	mBlurMaterial = make_shared<BlurMaterial>();
	mBlurMaterial->Initialise();

	mBlurMaterial->CreateVertexBuffer(mMesh.get(), mVertexBuffer.ReleaseAndGetAddressOf());

	RecreateHorizontalRenderTarget();
	ReCalculateSampleOffsetsAndWeight(mHorizontalRenderTarget->GetTexture()->GetWidth(), mHorizontalRenderTarget->GetTexture()->GetHeight());
}

void BlurComponent::OnWindowSizeChanged()
{
	PostProcessingComponent::OnWindowSizeChanged();

	RecreateHorizontalRenderTarget();
	ReCalculateSampleOffsetsAndWeight(mHorizontalRenderTarget->GetTexture()->GetWidth(), mHorizontalRenderTarget->GetTexture()->GetHeight());
}

void BlurComponent::OnDrawPostProcessing(const RenderState& renderState, const Texture2D* inputTexture)
{
	mBlurMaterial->SetCurrentTechnique("Blur");
	mBlurMaterial->SetSampleWeights(mSampleWeights);

	// horizontal blur to intermediate target
	DrawHorizontalBlur(renderState, inputTexture);
	// vertical blur to final target
	DrawVerticalBlur(renderState, mHorizontalRenderTarget->GetTexture().get());
}

void BlurComponent::DrawHorizontalBlur(const RenderState& renderState, const Texture2D* inputTexture)
{
	PipelineStateRestorer pipelineStateRestorer(renderState.deviceContext);

	mHorizontalRenderTarget->Begin(renderState);

	mBlurMaterial->SetSampleOffsets(mHorizontalSampleOffsets);
	DrawCurrentBlurTechnique(renderState, inputTexture);

	mHorizontalRenderTarget->End();
}

void BlurComponent::DrawVerticalBlur(const RenderState& renderState, const Texture2D* inputTexture)
{
	mBlurMaterial->SetSampleOffsets(mVerticalSampleOffsets);
	DrawCurrentBlurTechnique(renderState, inputTexture);
}

void BlurComponent::DrawCurrentBlurTechnique(const RenderState& renderState, const Texture2D* inputTexture)
{
	renderState.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const EffectPass* pass = mBlurMaterial->GetCurrentTechnique()->GetPasses()[0].get();
	ID3D11InputLayout* inputLayout = mBlurMaterial->GetInputLayout(pass);
	renderState.deviceContext->IASetInputLayout(inputLayout);

	mBlurMaterial->SetInputTexture(inputTexture);

	UInt32 stride = mBlurMaterial->VertexSize();
	UInt32 offset = mBlurMaterial->Offset();
	renderState.deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	renderState.deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	pass->Apply(0, renderState.deviceContext);
	renderState.deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void BlurComponent::RecreateHorizontalRenderTarget()
{
	mHorizontalRenderTarget = make_unique<RenderTarget>();
	// TODO: dont hardcode to Engine
	mHorizontalRenderTarget->Initialise(UInt32(Engine::Get()->GetViewport().width), UInt32(Engine::Get()->GetViewport().height));
}

inline float BlurComponent::CalculateWeight(float x) const
{
	return float(exp((x*x) / (2.f * mBlurAmount * mBlurAmount)));
}

void BlurComponent::ReCalculateSampleOffsetsAndWeight(const UInt32 width, const UInt32 height)
{
	float horizontalPixelToUV = 1.f / width;
	float verticalPixelToUV = 1.f / height;

	UInt32 numSamples = mBlurMaterial->NumSamples;

	mHorizontalSampleOffsets.clear();
	mVerticalSampleOffsets.clear();

	mHorizontalSampleOffsets.push_back(Vector2::Zero);
	mVerticalSampleOffsets.push_back(Vector2::Zero);

	// 0 already set, set 1 - N in each direction
	for (UInt32 iSample = 0; iSample < numSamples / 2; ++iSample)
	{
		float sampleOffsetPixels = iSample * 2.f + 1.5f;
		float horizontalOffsetUV = horizontalPixelToUV * sampleOffsetPixels;
		float verticalOffsetUV = verticalPixelToUV * sampleOffsetPixels;

		mHorizontalSampleOffsets.push_back({ horizontalOffsetUV, 0.f });
		mHorizontalSampleOffsets.push_back({ -horizontalOffsetUV, 0.f });

		mVerticalSampleOffsets.push_back({ 0.f, verticalOffsetUV });
		mVerticalSampleOffsets.push_back({ 0.f, -verticalOffsetUV });
	}

	mSampleWeights.clear();

	float zeroWeight = CalculateWeight(0.f);
	mSampleWeights.push_back(zeroWeight);

	float totalWeight = zeroWeight;

	for (UInt32 iSample = 0; iSample < numSamples / 2; ++iSample)
	{
		float weight = CalculateWeight(iSample + 1.f);

		// one for each direction
		mSampleWeights.push_back(weight);
		mSampleWeights.push_back(weight);

		totalWeight += weight * 2.f;
	}

	// normalize weights
	for (float& weight : mSampleWeights)
		weight /= totalWeight;
}
