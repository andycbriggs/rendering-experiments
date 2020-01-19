#pragma once

#include "BlurComponent.hpp"

class BloomMaterial;
class RenderTarget;
class Texture2D;

struct RenderState;

class BloomComponent : public BlurComponent
{
public:
	virtual void OnInitialise() override;
	virtual void OnWindowSizeChanged() override;
	virtual void OnDrawPostProcessing(const RenderState& localRenderState, const Texture2D* inputTexture) override;

private:
	void DrawCurrentBloomTechnique(const RenderState& localRenderState, const Texture2D* inputTexture);
	void ReCreateBloomRenderTarget();
	
	std::shared_ptr<BloomMaterial> mBloomMaterial;
	std::unique_ptr<RenderTarget> mBlurRenderTarget; // target for blur result
	std::unique_ptr<RenderTarget> mGlowRenderTarget; // intermediate glow extraction
};