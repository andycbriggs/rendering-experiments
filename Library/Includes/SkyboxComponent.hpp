#pragma once

#include "Helpers.hpp"
#include "Component.hpp"

class Effect;
class Model;
class SkyboxMaterial;
class TextureCube;

class SkyboxComponent : public Component
{
public:
    SkyboxComponent();
    ~SkyboxComponent();

    void OnInitialise() override;
    void OnDraw(const RenderState& renderState) override;

private:
    std::unique_ptr<SkyboxMaterial> mMaterial;
    std::unique_ptr<Effect> mEffect;
    std::unique_ptr<Model> mModel;
    std::unique_ptr<TextureCube> mTexture;

    UInt32 mIndexCount;
    ComPtr<ID3D11Buffer> mVertexBuffer;
    ComPtr<ID3D11Buffer> mIndexBuffer;
};