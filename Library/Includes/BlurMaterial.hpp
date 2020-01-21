#pragma once

#include "PostProcessingMaterial.hpp"

class BlurMaterial : public PostProcessingMaterial
{
public:
    const static UInt32 NumSamples = 9; // must match blur.fx

    virtual void Initialise() override;

    void SetSampleOffsets(const std::vector<Vector2>& offsets);
    void SetSampleWeights(const std::vector<float>& weights);
};