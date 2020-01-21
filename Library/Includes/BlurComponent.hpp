#pragma once

#include "PostProcessingComponent.hpp"

class BlurMaterial;
class RenderTarget;
class Texture2D;

struct RenderState;

class BlurComponent : public PostProcessingComponent
{
public:
    virtual void OnInitialise() override;
    virtual void OnWindowSizeChanged() override;
    virtual void OnDrawPostProcessing(const RenderState& renderState, const Texture2D* inputTexture) override;

    void SetBlurAmount(float blurAmount) { mBlurAmount = blurAmount; }

private:
    void DrawHorizontalBlur(const RenderState& renderState, const Texture2D* inputTexture);
    void DrawVerticalBlur(const RenderState& renderState, const Texture2D* inputTexture);
    void DrawCurrentBlurTechnique(const RenderState& renderState, const Texture2D* inputTexture);

    void RecreateHorizontalRenderTarget();
    void ReCalculateSampleOffsetsAndWeight(const UInt32 width, const UInt32 height);

private:
    float CalculateWeight(float x) const;

    float mBlurAmount = 4.f;

    std::vector<float> mSampleWeights;
    std::vector<Vector2> mHorizontalSampleOffsets;
    std::vector<Vector2> mVerticalSampleOffsets;

    std::unique_ptr<RenderTarget> mHorizontalRenderTarget;
    std::shared_ptr<BlurMaterial> mBlurMaterial;
};