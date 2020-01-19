#include "PostProcessingMaterial.hpp"

#include <VertexTypes.h>

#include "Helpers.hpp"
#include "Effect.hpp"
#include "EffectPass.hpp"
#include "EffectTechnique.hpp"
#include "EffectVariable.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "Texture2D.hpp"

using namespace std;

void PostProcessingMaterial::CreateVertexBuffer(Mesh* mesh, ID3D11Buffer** vertexBuffer)
{
    const vector<Vector3>& meshVertices =  mesh->GetVertices();
    UInt32 numVertices = UInt32(meshVertices.size());
    const vector<Vector3>& textureCoordinates = mesh->GetTextureCoordinates()[0];

    vector<VertexPositionTexture> vertices;

    for (UInt32 i = 0; i < numVertices; ++i)
        vertices.emplace_back(meshVertices[i], Vector2{ textureCoordinates[i].x, textureCoordinates[i].y });

    D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
    vertexBufferDesc.ByteWidth = VertexSize() * numVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

    D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
    vertexSubResourceData.pSysMem = vertices.data();

    HRESULT hr = Engine::Get()->Device()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer);
    ThrowIfFailed(hr, CreateBuffer);
}

UInt32 PostProcessingMaterial::VertexSize() const
{
    return UInt32(sizeof VertexPositionTexture);
}

void PostProcessingMaterial::SetInputTexture(const Texture2D* texture)
{
	EffectVariable* inputTexture = mEffect->GetVariable("InputTexture");
	inputTexture->SetValue(texture->GetShaderResourceView());
}