#include "ModelMaterial.hpp"

#include <assimp\scene.h>

#include "Helpers.hpp"

using namespace std;

namespace
{
    inline aiTextureType AiTextureTypeFromTextureType(TextureType& aiTextureType)
    {
        switch (aiTextureType)
        {
        case TextureTypeDiffuse:
            return aiTextureType_DIFFUSE;
        case TextureTypeSpecularMap:
            return aiTextureType_SPECULAR;
        case TextureTypeAmbient:
            return aiTextureType_AMBIENT;
        case TextureTypeEmissive:
            return aiTextureType_EMISSIVE;
        case TextureTypeHeightMap:
            return aiTextureType_HEIGHT;
        case TextureTypeNormalMap:
            return aiTextureType_NORMALS;
        case TextureTypeSpecularPowerMap:
            return aiTextureType_SHININESS;
        case TextureTypeDisplacementMap:
            return aiTextureType_DISPLACEMENT;
        case TextureTypeLightMap:
            return aiTextureType_LIGHTMAP;
        default:
            return aiTextureType_UNKNOWN;
        }
    }
}

ModelMaterial::ModelMaterial(aiMaterial* material)
{
    aiString name;
    if (AI_SUCCESS != material->Get(AI_MATKEY_NAME, name))
        ThrowFunctionError(L"Failed to get material name", Get);
    mName = name.C_Str();

    // iterate every known texture type
    for (TextureType textureType = TextureType(0); textureType < TextureTypeUnknown; textureType = TextureType(textureType + 1))
    {
        UInt32 textureCount = material->GetTextureCount(AiTextureTypeFromTextureType(textureType));

        if (textureCount > 0)
        {
            vector<string> textures;
            for (UInt32 textureIndex = 0; textureIndex < textureCount; ++textureIndex)
            {
                aiString path;
                if (AI_SUCCESS == material->GetTexture(AiTextureTypeFromTextureType(textureType), textureIndex, &path))
                {
                    textures.push_back(path.C_Str());
                }
            }
            mTextures.emplace(textureType, textures);
        }
    }
}