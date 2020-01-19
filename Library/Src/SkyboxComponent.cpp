#include "SkyboxComponent.hpp"

#include <CommonStates.h>

#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "EffectVariable.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "PipelineStateRestorer.hpp"
#include "RenderState.hpp"
#include "SkyboxMaterial.hpp"
#include "TextureCube.hpp"

using namespace std;

SkyboxComponent::SkyboxComponent() = default;
SkyboxComponent::~SkyboxComponent() = default;

void SkyboxComponent::OnInitialise() 
{
    mModel = make_unique<Model>("Content\\Models\\Sphere.obj");
    mTexture = make_unique<TextureCube>("Content\\Textures\\SpaceSkybox.dds");

    mMaterial = make_unique<SkyboxMaterial>();
    mMaterial->Initialise();

    Mesh* mesh = mModel->Meshes().at(0).get();
    mesh->CreateIndexBuffer(Engine::Get()->Device(), mIndexBuffer.ReleaseAndGetAddressOf());
    mMaterial->CreateVertexBuffer(mesh, mVertexBuffer.ReleaseAndGetAddressOf());
    mIndexCount = UInt32(mesh->GetIndices().size());
}

void SkyboxComponent::OnDraw(const RenderState& renderState)
{
	PipelineStateRestorer pipelineStateRestorer(renderState.deviceContext);

	renderState.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    const EffectPass* pass = mMaterial->GetCurrentTechnique()->GetPasses()[0].get();
    ID3D11InputLayout* inputLayout = mMaterial->GetInputLayout(pass);
	renderState.deviceContext->IASetInputLayout(inputLayout);

	mMaterial->SetWorldViewProjection(renderState.worldViewProjectionMatrix);
	mMaterial->SetTexture(mTexture.get());

	

    UInt32 stride = mMaterial->VertexSize();
    UInt32 offset = mMaterial->Offset();
	renderState.deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	renderState.deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    pass->Apply(0, renderState.deviceContext);
	renderState.deviceContext->DrawIndexed(mIndexCount, 0, 0);
}