#pragma once

#include "PostProcessingMaterial.hpp"

class BloomMaterial : public PostProcessingMaterial
{
public:
    void Initialise() override;

    void SetSceneTexture(const Texture2D* texture);
};