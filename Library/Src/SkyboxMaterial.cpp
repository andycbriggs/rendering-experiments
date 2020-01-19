#include "SkyboxMaterial.hpp"

#include <VertexTypes.h>

#include "Effect.hpp"
#include "EffectVariable.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "TextureCube.hpp"

using namespace std;
using namespace DirectX;

void SkyboxMaterial::Initialise()
{
    mEffect = make_unique<Effect>(L"Content\\Effects\\Skybox.cso");

    mWvp = mEffect->GetVariable("WorldViewProjection");
    mTexture = mEffect->GetVariable("SkyboxTexture");

    CreateInputLayout("main11", "p0", VertexPosition::InputElements, VertexPosition::InputElementCount);
}

void SkyboxMaterial::CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer)
{
    const auto& meshVertices =  mesh->GetVertices();
    UInt32 nVertices = UInt32(meshVertices.size());
    vector<VertexPosition> vertices;

    for (UInt32 i = 0; i < nVertices; ++i)
        vertices.emplace_back(meshVertices[i]);

    D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
    vertexBufferDesc.ByteWidth = VertexSize() * nVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

    D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
    vertexSubResourceData.pSysMem = vertices.data();

    HRESULT hr = Engine::Get()->Device()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer);
    ThrowIfFailed(hr, CreateBuffer);
}

void SkyboxMaterial::SetWorldViewProjection(const Matrix& wvp)
{
    mWvp->SetValue(wvp);
}

void SkyboxMaterial::SetTexture(const TextureCube* texture)
{
    mTexture->SetValue(texture->GetShaderResourceView());
}

UInt32 SkyboxMaterial::VertexSize() const
{
    return UInt32(sizeof VertexPosition);
}