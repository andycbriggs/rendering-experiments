#include "MeshComponent.hpp"

#include "CameraComponent.hpp"
#include "DirectionalLight.hpp"
#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "EffectVariable.hpp"
#include "Engine.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "PhongMaterial.hpp"
#include "PointLight.hpp"
#include "RenderState.hpp"
#include "Texture2D.hpp"
#include "Timer.hpp"

using namespace std;
using namespace DirectX;

MeshComponent::MeshComponent() {};
MeshComponent::~MeshComponent() {};

void MeshComponent::OnInitialise()
{
    mMaterial = make_shared<PhongMaterial>();
    mMaterial->Initialise();
}

void MeshComponent::OnUpdate(const TimerTime& time)
{
}

void MeshComponent::OnDraw(const RenderState& renderState)
{
	if (!mMesh)
		return; // no mesh to render

	// check if buffers need re-creating
	if (!mIndexBuffer || !mVertexBuffer || !mIndexCount)
	{
		mMesh->CreateIndexBuffer(Engine::Get()->Device(), mIndexBuffer.ReleaseAndGetAddressOf());
		mMaterial->CreateVertexBuffer(mMesh.get(), mVertexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = UInt32(mMesh->GetIndices().size());
	}

	renderState.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    const EffectPass* pass = mMaterial->GetCurrentTechnique()->GetPasses()[0].get();
    ID3D11InputLayout* inputLayout = mMaterial->GetInputLayout(pass);
	renderState.deviceContext->IASetInputLayout(inputLayout);

	mMaterial->SetCameraTranslation(renderState.cameraTranslation);
    mMaterial->SetWorldViewProjection(renderState.worldViewProjectionMatrix);
    mMaterial->SetWorld(renderState.worldMatrix);
    mMaterial->SetDiffuseTexture(mTexture.get());

	if (renderState.ambientLight)
		mMaterial->SetAmbientLightColor(renderState.ambientLight->mColor);
	else
		mMaterial->SetAmbientLightColor(Color(Colors::White));

	if (renderState.directionalLight)
	{
		mMaterial->SetDirectionalLightColor(renderState.directionalLight->mColor);
		mMaterial->SetDirectionalLightDirection(renderState.directionalLight->mDirection);
	}
	else
	{
		mMaterial->SetDirectionalLightColor(Color(Colors::Black));
	}

	if (renderState.pointLight)
	{
		mMaterial->SetPointLightColor(renderState.pointLight->mColor);
		mMaterial->SetPointLightPosition(renderState.pointLight->mTranslation);
		mMaterial->SetPointLightRadius(renderState.pointLight->mRadius);
	}
	else
	{
		mMaterial->SetPointLightColor(Color(Colors::Black));
	}

    UInt32 stride = mMaterial->VertexSize();
    UInt32 offset = mMaterial->Offset();
	renderState.deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	renderState.deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    pass->Apply(0, renderState.deviceContext);
	renderState.deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void MeshComponent::SetMesh(std::shared_ptr<Mesh>& mesh)
{
	mMesh = mesh;

	// mesh has changed so release buffers
	mIndexBuffer = nullptr;
	mVertexBuffer = nullptr;
	mIndexCount = 0;
}

void MeshComponent::SetTexture(std::shared_ptr<Texture2D>& texture)
{
	mTexture = texture;
}
