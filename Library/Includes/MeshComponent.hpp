#pragma once

#include "Component.hpp"

class PhongMaterial;
class Mesh;
class Texture2D;

class MeshComponent : public Component
{
public:
    MeshComponent();
    ~MeshComponent();

    void OnInitialise() override;
    void OnUpdate(const TimerTime& time) override;
    void OnDraw(const RenderState& renderState) override;

    void SetMesh(std::shared_ptr<Mesh>& mesh);
    void SetTexture(std::shared_ptr<Texture2D>& texture);

private:
    std::shared_ptr<PhongMaterial> mMaterial;
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Texture2D> mTexture;

    UInt32 mIndexCount = 0;
    ComPtr<ID3D11Buffer> mVertexBuffer;
    ComPtr<ID3D11Buffer> mIndexBuffer;
};