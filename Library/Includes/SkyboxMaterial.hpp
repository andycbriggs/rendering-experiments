#pragma once

#include "Material.hpp"

class EffectVariable;
class TextureCube;

struct ID3D11Buffer;

class SkyboxMaterial : public Material
{
public:
    void Initialise() override;
    void CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer) override;
    UInt32 VertexSize() const override;

    void SetWorldViewProjection(const Matrix& wvp);
    void SetTexture(const TextureCube* texture);

private:
    EffectVariable* mWvp;
    EffectVariable* mTexture;
};