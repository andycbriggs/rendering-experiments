#pragma once

#include "DirectXTKIncludes.hpp"

struct ID3D11Buffer;
struct ID3D11Device;

struct aiMesh;

class Mesh
{
public:
    Mesh();
    Mesh(aiMesh* mesh);

    static std::shared_ptr<Mesh> CreatePlane();
    static std::shared_ptr<Mesh> CreateCube();
    static std::shared_ptr<Mesh> CreateUnwrappedCube();
    static std::shared_ptr<Mesh> CreateUVSphere();

    void CreateIndexBuffer(ID3D11Device* device, ID3D11Buffer** indexBuffer);

    const std::vector<Vector3>& GetVertices() { return mVertices; }
    const std::vector<Vector3>& GetNormals() { return mNormals; }
    const std::vector<Vector3>& GetTangents() { return mTangents; }
    const std::vector<Vector3>& GetBiNormals() { return mBiNormals; }
    const std::vector<UInt32>& GetIndices() { return mIndices; }

    const std::vector<std::vector<Vector3>>& GetTextureCoordinates() { return mTextureCoordinates; }
    const std::vector<std::vector<Color>>& GetVertexColors() { return mVertexColors; }
    const UInt32& GetFaceCount() { return mFaceCount; }

private:
    std::vector<Vector3> mVertices;
    std::vector<Vector3> mNormals;
    std::vector<Vector3> mTangents;
    std::vector<Vector3> mBiNormals;
    std::vector<UInt32> mIndices;

    std::vector<std::vector<Vector3>> mTextureCoordinates;
    std::vector<std::vector<Color>> mVertexColors;
    UInt32 mFaceCount;
};