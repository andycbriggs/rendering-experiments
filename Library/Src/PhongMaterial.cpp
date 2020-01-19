#include "PhongMaterial.hpp"

#include <VertexTypes.h>

#include "Effect.hpp"
#include "EffectVariable.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "Texture2D.hpp"

using namespace std;

PhongMaterial::PhongMaterial() = default;
PhongMaterial::~PhongMaterial() = default;

void PhongMaterial::Initialise()
{
    mEffect = make_unique<Effect>(L"Content\\Effects\\Phong.cso");
    CreateInputLayout("main11", "p0", VertexPositionNormalTexture::InputElements, VertexPositionNormalTexture::InputElementCount);
}

void PhongMaterial::CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer)
{
    const auto& meshVertices = mesh->GetVertices();
    UInt32 numVertices = UInt32(meshVertices.size());
    const auto& meshTextureCoordinates = mesh->GetTextureCoordinates()[0];
    const auto& meshNormals = mesh->GetNormals(); 

    vector<VertexPositionNormalTexture> vertices;

    for (UInt32 i = 0; i < numVertices; ++i)
    {
        vertices.emplace_back(meshVertices[i],
            meshNormals[i],
            Vector2{meshTextureCoordinates[i].x, meshTextureCoordinates[i].y});
    }

    D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
    vertexBufferDesc.ByteWidth = VertexSize() * numVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

    D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
    vertexSubResourceData.pSysMem = vertices.data();

    HRESULT hr = Engine::Get()->Device()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer);
    ThrowIfFailed(hr, CreateBuffer);
}

UInt32 PhongMaterial::VertexSize() const
{
    return UInt32(sizeof VertexPositionNormalTexture);
}

void PhongMaterial::SetDiffuseTexture(const Texture2D* texture)
{
    EffectVariable* effectVariable = mEffect->GetVariable("DiffuseTexture");
    effectVariable->SetValue(texture->GetShaderResourceView());
}

void PhongMaterial::SetWorldViewProjection(const Matrix& wvpMatrix)
{
    const EffectVariable* effectVariable = mEffect->GetVariable("WorldViewProjection");
    effectVariable->SetValue(wvpMatrix);
}

void PhongMaterial::SetWorld(const Matrix& world)
{
    const EffectVariable* effectVariable = mEffect->GetVariable("World");
    effectVariable->SetValue(world);
}

void PhongMaterial::SetAmbientLightColor(const Color& color)
{
    EffectVariable* ambientColor = mEffect->GetVariable("AmbientLightColor");
    ambientColor->SetValue(color.ToVector4());
}

void PhongMaterial::SetDirectionalLightColor(const Color& color)
{
    EffectVariable* effectVariable = mEffect->GetVariable("DirectionalLightColor");
    effectVariable->SetValue(color.ToVector4());
}

void PhongMaterial::SetDirectionalLightDirection(const Vector3& direction)
{
    EffectVariable* effectVariable = mEffect->GetVariable("DirectionalLightDirection");
    effectVariable->SetValue(direction);
}

void PhongMaterial::SetPointLightPosition(const Vector3& position)
{
    EffectVariable* effectVariable = mEffect->GetVariable("PointLightPosition");
    effectVariable->SetValue(position);
}

void PhongMaterial::SetPointLightColor(const Color& color)
{
    EffectVariable* effectVariable = mEffect->GetVariable("PointLightColor");
    effectVariable->SetValue(color.ToVector4());
}

void PhongMaterial::SetPointLightRadius(const float radius)
{
    EffectVariable* effectVariable = mEffect->GetVariable("PointLightRadius");
    effectVariable->SetValue(radius);
}

void PhongMaterial::SetCameraTranslation(const Vector3& position)
{
    EffectVariable* effectVariable = mEffect->GetVariable("CameraTranslation");
    effectVariable->SetValue(position);
}
