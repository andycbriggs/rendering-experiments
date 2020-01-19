#pragma once

#include "Material.hpp"

class Effect;
class Mesh;
class Texture2D;

struct ID3D11Buffer;

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    ~PhongMaterial();

    void Initialise() override;
    void CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer) override;
    UInt32 VertexSize() const override;

    void SetWorldViewProjection(const Matrix& wvpMatrix);
    void SetWorld(const Matrix& world);
    void SetDiffuseTexture(const Texture2D* texture);
    void SetAmbientLightColor(const Color& color);
    void SetDirectionalLightColor(const Color& color);
    void SetDirectionalLightDirection(const Vector3& direction);
    void SetPointLightPosition(const Vector3& position);
    void SetPointLightColor(const Color& color);
    void SetPointLightRadius(const float radius);
    void SetCameraTranslation(const Vector3& position);
};