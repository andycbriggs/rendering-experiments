#include "Model.hpp"

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "Helpers.hpp"
#include "Mesh.hpp"
#include "ModelMaterial.hpp"

using namespace std;

Model::Model(std::string filename)
{
    Assimp::Importer importer;

    UInt32 flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder | aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(filename, flags);
    if (scene == nullptr)
        ThrowFunctionError(toStdWString(importer.GetErrorString()), ReadFile);

    if (scene->HasMaterials())
    {
       for (UInt32 i = 0; i < scene->mNumMaterials; ++i)
       {
           mMaterials.emplace_back(make_unique<ModelMaterial>(scene->mMaterials[i]));
       }
    }

    if (scene->HasMeshes())
    {
       for (UInt32 i = 0; i < scene->mNumMeshes; ++i)
       {
           mMeshes.emplace_back(make_unique<Mesh>(scene->mMeshes[i]));
       }
    }
}

Model::~Model()
{

}