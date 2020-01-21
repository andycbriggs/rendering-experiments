#include "LightingComponent.hpp"

#include "Engine.hpp"
#include "PipelineStateRestorer.hpp"
#include "RenderState.hpp"

LightingComponent::LightingComponent()
    : Component()
{
    mShadowMapRenderTarget.Initialise(1024, 1024);
    mShadowMapRenderTarget.SetClearColor(Color(Colors::Black));
}

void LightingComponent::OnBeginDraw(const RenderState& localRenderState)
{
    RenderState spotLightRenderState = localRenderState;

    Matrix viewMatrix = Matrix::CreateTranslation(mSpotLight.mTranslation).Invert() * Matrix::CreateFromYawPitchRoll(mSpotLight.mDirection.x, mSpotLight.mDirection.y, mSpotLight.mDirection.z).Transpose();
    Matrix projectionMatrix = Matrix::CreatePerspectiveFieldOfView(mSpotLight.mOuterAngle, 1.f, 0.001f, 10000.f);
    Matrix viewProjectionMatrix = viewMatrix * projectionMatrix;

    spotLightRenderState.viewProjectionMatrix = viewProjectionMatrix;
    spotLightRenderState.worldViewProjectionMatrix = spotLightRenderState.worldMatrix * viewProjectionMatrix;
    spotLightRenderState.cameraTranslation = mSpotLight.mTranslation;

    mShadowMapRenderTarget.Begin(spotLightRenderState);

    DrawChildren(spotLightRenderState);

    mShadowMapRenderTarget.End();
}

void LightingComponent::OnUpdateLocalRenderState(const RenderState& parentRenderState, RenderState& localRenderState)
{
    localRenderState.ambientLight = &mAmbientLight;
    localRenderState.directionalLight = &mDirectionalLight;
    localRenderState.pointLight = &mPointLight;
}
