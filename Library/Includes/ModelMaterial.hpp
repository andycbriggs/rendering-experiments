#pragma once

struct aiMaterial;

class Model;

enum TextureType
{
    TextureTypeDiffuse = 0,
    TextureTypeSpecularMap,
    TextureTypeAmbient,
    TextureTypeEmissive,
    TextureTypeHeightMap,
    TextureTypeNormalMap,
    TextureTypeSpecularPowerMap,
    TextureTypeDisplacementMap,
    TextureTypeLightMap,
    TextureTypeUnknown
};

class ModelMaterial
{
public:
    ModelMaterial(aiMaterial* material);

private:
    std::string mName;
    std::map<TextureType, std::vector<std::string>> mTextures;
};