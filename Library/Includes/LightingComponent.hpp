#pragma once

#include "Component.hpp"
#include "DirectionalLight.hpp"
#include "Light.hpp"
#include "PointLight.hpp"
#include "RenderTarget.hpp"
#include "SpotLight.hpp"

struct RenderState;

class LightingComponent : public Component
{
public:
	LightingComponent();

	void SetAmbientColor(Color color) { mAmbientLight.mColor = color; }
	
	void SetDirectionalColor(Color color) { mDirectionalLight.mColor = color; }
	void SetDirectionalDirection(Vector3 direction) { mDirectionalLight.mDirection = direction; }
	
	void SetPointLightColor(Color color) { mPointLight.mColor = color; }
	void SetPointLightTranslation(Vector3 translation) { mPointLight.mTranslation = translation; }
	void SetPointLightRadius(float radius) { mPointLight.mRadius = radius; }

	void SetSpotLightColor(Color color) { mSpotLight.mColor = color; }
	void SetSpotLightTranslation(Vector3 translation) { mSpotLight.mTranslation = translation; }
	void SetSpotLightDirection(Vector3 rotation) { mSpotLight.mDirection = rotation; }
	void SetSpotLightCone(float innerAngle, float outerAngle) { mSpotLight.mInnerAngle = innerAngle; mSpotLight.mOuterAngle = outerAngle; }
	void SetSpotLightRadius(float radius) { mSpotLight.mRadius = radius; }

	RenderTarget& ShadowMapRenderTarget() { return mShadowMapRenderTarget; }

private:
	void OnBeginDraw(const RenderState& renderState) override;
	void OnUpdateLocalRenderState(const RenderState& parentRenderState, RenderState& localRenderState) override;

	Light mAmbientLight;
	DirectionalLight mDirectionalLight;
	PointLight mPointLight;
	SpotLight mSpotLight;

	RenderTarget mShadowMapRenderTarget;
};