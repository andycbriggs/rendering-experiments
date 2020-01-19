#pragma once

#include "PostProcessingComponent.hpp"

class ColorFilterMaterial;
class Texture2D;

struct RenderState;

class ColorFilterComponent : public PostProcessingComponent
{
public:
	enum class Filter
	{
		NoEffect,
		Greyscale,
		Sepia,
		Invert
	};

    ColorFilterComponent::ColorFilterComponent();
    ColorFilterComponent::~ColorFilterComponent();

    void OnDrawPostProcessing(const RenderState& renderState, const Texture2D* inputTexture) override;
    void OnInitialise() override;

	void SetFilter(Filter filter);

private:
	Filter mCurrentFilter = Filter::NoEffect;

    std::unique_ptr<ColorFilterMaterial> mMaterial;
};