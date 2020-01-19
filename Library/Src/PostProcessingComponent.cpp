#include "PostProcessingComponent.hpp"

#include "Engine.hpp"
#include "Mesh.hpp"
#include "RenderTarget.hpp"
#include "Material.hpp"

using namespace std;

PostProcessingComponent::PostProcessingComponent() : Component() {};
PostProcessingComponent::~PostProcessingComponent() {};

void PostProcessingComponent::OnInitialise()
{
	mMesh = Mesh::CreatePlane();

	mMesh->CreateIndexBuffer(Engine::Get()->Device(), mIndexBuffer.ReleaseAndGetAddressOf());
	mIndexCount = UInt32(mMesh->GetIndices().size());
}

void PostProcessingComponent::OnBeginDraw(const RenderState& renderState)
{
	// ensure render target exists
	if (!mRenderTarget)
		RecreateRenderTarget();

	mRenderTarget->Begin(renderState); // begin drawing children
}

void PostProcessingComponent::OnEndDraw(const RenderState& renderState)
{
	mRenderTarget->End(); // end drawing children

	OnDrawPostProcessing(renderState, mRenderTarget->GetTexture().get());
}

void PostProcessingComponent::OnWindowSizeChanged()
{
	RecreateRenderTarget();
}

void PostProcessingComponent::RecreateRenderTarget()
{
	mRenderTarget = make_unique<RenderTarget>();
	mRenderTarget->Initialise(UInt32(Engine::Get()->GetViewport().width), UInt32(Engine::Get()->GetViewport().height));
}