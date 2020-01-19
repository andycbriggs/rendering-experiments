#pragma once

#include "Material.hpp"

struct ID3D11Buffer;

class Texture2D;

class PostProcessingMaterial : public Material
{
public:
    void CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer) override;
    UInt32 VertexSize() const override;

	virtual void SetInputTexture(const Texture2D* texture);
};