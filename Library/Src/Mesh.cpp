#include "Mesh.hpp"

#include <assimp\scene.h>

#include "Helpers.hpp"

using namespace std;

/*static*/ shared_ptr<Mesh> Mesh::CreatePlane()
{
    shared_ptr<Mesh> mesh = make_shared<Mesh>();
    
    mesh->mVertices = {
        { -0.5f,  0.5f,  0.f }, // 0
        {  0.5f,  0.5f,  0.f }, // 1
        { -0.5f, -0.5f,  0.f }, // 2
        {  0.5f, -0.5f,  0.f }, // 3
        { -0.5f,  0.5f,  0.f }, // 4
        {  0.5f,  0.5f,  0.f }, // 5
        { -0.5f, -0.5f,  0.f }, // 6
        {  0.5f, -0.5f,  0.f }, // 7
    };

    // tl, tr, bl
    // bl, tr, br
    mesh->mIndices = {
        0, 1, 2, // front
        2, 1, 3,
        6, 5, 4, // back
        7, 5, 6,
    };

    mesh->mNormals = {
        Vector3::Backward,
        Vector3::Backward,
        Vector3::Backward,
        Vector3::Backward,
        Vector3::Forward,
        Vector3::Forward,
        Vector3::Forward,
        Vector3::Forward,
    };

    mesh->mTextureCoordinates = { {
        { 0.f, 0.f, 0.f }, // tl
        { 1.f, 0.f, 0.f }, // tr
        { 0.f, 1.f, 0.f }, // bl
        { 1.f, 1.f, 0.f }, // br
        { 0.f, 0.f, 0.f }, // tl
        { 1.f, 0.f, 0.f }, // tr
        { 0.f, 1.f, 0.f }, // bl
        { 1.f, 1.f, 0.f }, // br
    } };
    
    return mesh;
}

/*static*/ shared_ptr<Mesh> Mesh::CreateCube()
{
    const static std::vector<Vector3> uniqueUVs = {
        { 0.f, 1.f, 0.f }, // tl
        { 1.f, 1.f, 0.f }, // tr
        { 0.f, 0.f, 0.f }, // bl
        { 1.f, 0.f, 0.f }, // br
    };

    const static std::vector<Vector3> uniqueVerts = {
        { -0.5f,  0.5f,  0.5f }, // 0
        {  0.5f,  0.5f,  0.5f }, // 1
        { -0.5f, -0.5f,  0.5f }, // 2
        {  0.5f, -0.5f,  0.5f }, // 3
        
        { -0.5f,  0.5f, -0.5f }, // 4
        {  0.5f,  0.5f, -0.5f }, // 5
        { -0.5f, -0.5f, -0.5f }, // 6
        {  0.5f, -0.5f, -0.5f }, // 7
    };

    // tl, tr, bl
    // bl, tr, br
    const static vector<UInt32> instancedVerts = {
        0, 1, 2, // front
        2, 1, 3,
        5, 4, 7, // back
        7, 4, 6,
        4, 0, 6, // left
        6, 0, 2,
        1, 5, 3, // right
        3, 5, 7,
        4, 5, 0, // top
        0, 5, 1,
        2, 3, 6, // bottom
        6, 3, 7,
    };

    // tl, tr, bl
    // bl, tr, br
    const static vector<UInt32> instancedUVs = {
        0, 1, 2,
        2, 1, 3,
    };

    const static std::vector<Vector3> uniqueNormals = {
        Vector3::Backward,
        Vector3::Forward,
        Vector3::Left,
        Vector3::Right,
        Vector3::Up,
        Vector3::Down,
    };

    shared_ptr<Mesh> mesh = make_shared<Mesh>();

    mesh->mTextureCoordinates.resize(1); // push an empty first set of UVs we can fill

    for (UInt32 i = 0; i < instancedVerts.size(); ++i)
    {
        mesh->mVertices.push_back(uniqueVerts[instancedVerts[i]]);
        mesh->mIndices.push_back(i);
        mesh->mTextureCoordinates.front().push_back(uniqueUVs[instancedUVs[i % 6]]);
        mesh->mNormals.push_back(uniqueNormals[i / 6]);
    }

    return mesh;
}

/*static*/ shared_ptr<Mesh> Mesh::CreateUnwrappedCube()
{    
    //    [ ]
    // [ ][ ][ ][ ]
    //    [ ]

    auto uv = [](int row, int column) -> Vector3 {
        return { float(column) / 4.f, float(row) / 3.f, 0.f };
    };

    const static std::vector<Vector3> uniqueVerts = {
        { -0.5f,  0.5f,  0.5f }, // 0
        {  0.5f,  0.5f,  0.5f }, // 1
        { -0.5f, -0.5f,  0.5f }, // 2
        {  0.5f, -0.5f,  0.5f }, // 3

        { -0.5f,  0.5f, -0.5f }, // 4
        {  0.5f,  0.5f, -0.5f }, // 5
        { -0.5f, -0.5f, -0.5f }, // 6
        {  0.5f, -0.5f, -0.5f }, // 7
    };

    // tl, tr, bl
    // bl, tr, br
    const static vector<UInt32> instancedVerts = {
        0, 1, 2, // front
        2, 1, 3,
        5, 4, 7, // back
        7, 4, 6,
        4, 0, 6, // left
        6, 0, 2,
        1, 5, 3, // right
        3, 5, 7,
        4, 5, 0, // top
        0, 5, 1,
        2, 3, 6, // bottom
        6, 3, 7,
    };

    // tl, tr, bl
    // bl, tr, br
    const static vector<Vector3> uvs = {
        uv(1, 2), uv(1, 3), uv(2, 2), // front
        uv(2, 2), uv(1, 3), uv(2, 3),

        uv(1, 0), uv(1, 1), uv(2, 0), // back
        uv(2, 0), uv(1, 1), uv(2, 1),

        uv(1, 1), uv(1, 2), uv(2, 1), // left
        uv(2, 1), uv(1, 2), uv(2, 2),

        uv(1, 3), uv(1, 4), uv(2, 3), // right
        uv(2, 3), uv(1, 4), uv(2, 4),

        uv(0, 1), uv(0, 2), uv(1, 1), // top
        uv(1, 1), uv(0, 2), uv(1, 2),

        uv(2, 1), uv(2, 2), uv(3, 1), // bottom
        uv(3, 1), uv(2, 2), uv(3, 2),
    };

    const static std::vector<Vector3> uniqueNormals = {
        Vector3::Backward,
        Vector3::Forward,
        Vector3::Left,
        Vector3::Right,
        Vector3::Up,
        Vector3::Down,
    };

    shared_ptr<Mesh> mesh = make_shared<Mesh>();

    for (UInt32 i = 0; i < instancedVerts.size(); ++i)
    {
        mesh->mVertices.push_back(uniqueVerts[instancedVerts[i]]);
        mesh->mIndices.push_back(i);
        mesh->mNormals.push_back(uniqueNormals[i / 6]);
    }

    mesh->mTextureCoordinates.push_back(uvs);

    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateUVSphere()
{
    float radius = .5f;
    int widthSegments = 64;
    int heightSegments = 64;

    float phiStart = 0;
    float phiLength = PI * 2.f;
    float thetaStart = 0;
    float thetaLength = PI;
    float thetaEnd = thetaStart + thetaLength;

    shared_ptr<Mesh> mesh = make_shared<Mesh>();

    mesh->mTextureCoordinates.resize(1); // push an empty first set of UVs we can fill

    int index = 0;
    vector<vector<int>> grid; // 2d array of x, y to vertex index
    for (int x = 0; x <= widthSegments; ++x)
    {
        float v = float(x) / widthSegments;

        vector<int> rowIndexes;

        for (int y = 0; y <= heightSegments; ++y)
        {
            float u = float(y) / heightSegments;

            Vector3 vertex;

            vertex.x = radius * cos(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength);
            vertex.y = radius * cos(thetaStart + v * thetaLength);
            vertex.z = radius * sin(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength);

            Vector3 normal = vertex;
            normal.Normalize();

            mesh->mVertices.push_back(vertex);
            mesh->mNormals.push_back(normal);
            mesh->mTextureCoordinates.back().push_back({ 1 - u, v, 0.f });

            rowIndexes.push_back(index);
            ++index;
        }

        grid.push_back(rowIndexes);
    }

    // build index buffer
    for (int x = 0; x < widthSegments; ++x)
    {
        for (int y = 0; y < heightSegments; ++y)
        {
            int a = grid[y][x + 1];
            int b = grid[y][x];
            int c = grid[y + 1][x];
            int d = grid[y + 1][x + 1];

            if (y != 0 || thetaStart > 0)
            {
                mesh->mIndices.push_back(a);
                mesh->mIndices.push_back(b);
                mesh->mIndices.push_back(d);
            }
            if (y != heightSegments - 1 || thetaEnd < PI)
            {
                mesh->mIndices.push_back(b);
                mesh->mIndices.push_back(c);
                mesh->mIndices.push_back(d);
            }
        }
    }

    return mesh;
}


Mesh::Mesh()
{
}

Mesh::Mesh(aiMesh* mesh)
{
    for (UInt32 i = 0; i < mesh->mNumVertices; ++i)
    {
        mVertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    }

    if (mesh->HasNormals())
    {
        for (UInt32 i = 0; i < mesh->mNumVertices; ++i)
        {
            mNormals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
    }

    if (mesh->HasTangentsAndBitangents())
    {
        for (UInt32 i = 0; i < mesh->mNumVertices; ++i)
        {
            mTangents.emplace_back(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            mBiNormals.emplace_back(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
    }
    
    UInt32 uvChannelCount = mesh->GetNumUVChannels();
    for (UInt32 uvChannel = 0; uvChannel < uvChannelCount; ++uvChannel)
    {
        vector<Vector3> textureCoordinates;
        for (UInt32 i = 0; i < mesh->mNumVertices; ++i)
        {
            textureCoordinates.emplace_back(mesh->mTextureCoords[uvChannel][i].x, mesh->mTextureCoords[uvChannel][i].y, mesh->mTextureCoords[uvChannel][i].z);
        }
        mTextureCoordinates.emplace_back(textureCoordinates);
    }
    
    UInt32 colorChannelCount = mesh->GetNumColorChannels();
    for (UInt32 colorChannel = 0; colorChannel < colorChannelCount; ++colorChannel)
    {
        vector<Color> vertexColors;
        for (UInt32 i = 0; i < mesh->mNumVertices; ++i)
        {
            vertexColors.emplace_back(mesh->mColors[colorChannel][i].r, mesh->mColors[colorChannel][i].g, mesh->mColors[colorChannel][i].b, mesh->mColors[colorChannel][i].a);
        }
        mVertexColors.emplace_back(vertexColors);
    }

    if (mesh->HasFaces())
    {
        mFaceCount = mesh->mNumFaces;
        for (UInt32 face = 0; face < mFaceCount; ++face)
        {
            for (UInt32 i = 0; i < mesh->mFaces[face].mNumIndices; ++i)
            {
                mIndices.emplace_back(mesh->mFaces[face].mIndices[i]);
            }
        }
    }
}

void Mesh::CreateIndexBuffer(ID3D11Device* device, ID3D11Buffer** indexBuffer)
{
    // create index buffer
    D3D11_BUFFER_DESC indexBufferDesc = { 0 };
    indexBufferDesc.ByteWidth = sizeof UInt32 * UInt32(mIndices.size());
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubResourceData = { 0 };
    indexSubResourceData.pSysMem = mIndices.data();

    HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer);
    ThrowIfFailed(hr, CreateBuffer);

    SetDebugObjectName(*indexBuffer, "Index Buffer");
}
