#pragma once

class Mesh;
class ModelMaterial;

class Model
{
public:
    Model(std::string filename);
    ~Model();

    std::vector<std::unique_ptr<Mesh>>& Meshes() { return mMeshes; };
private:
    std::vector<std::unique_ptr<Mesh>> mMeshes;
    std::vector<std::unique_ptr<ModelMaterial>> mMaterials;
};