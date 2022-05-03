#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Inspiracion: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h

struct vec3
{
    float x, y, z;
};

struct vec2
{
    float x, y;
};

struct mesh
{
    std::string Name;
    std::vector<vec3> Vertices;
    std::vector<vec3> Normals;
    std::vector<vec2> TextureCoordinates;
    std::vector<unsigned int> Indices;
};

struct model
{
    std::string Name;
    std::vector<mesh> Meshes;
    // Materiales
    // std::string Directory;
    // std::vector<texture> Textures;
};

//
// Globals
//

model Model;

//
// Functions
//

void ProcessMesh(const aiMesh *CurrentMesh, std::vector<mesh> &Meshes)
{
    mesh Result;
    Result.Name = CurrentMesh->mName.C_Str();

    // Extract Mesh Vertices
    if(CurrentMesh->HasPositions())
    {
        for(unsigned int j = 0; j < CurrentMesh->mNumVertices; j++)
        {
            vec3 Vertex =
            {
                CurrentMesh->mVertices[j].x,
                CurrentMesh->mVertices[j].y,
                CurrentMesh->mVertices[j].z,
            };
            Result.Vertices.push_back(Vertex);
        }
    }

    // Extract Mesh Normals
    if(CurrentMesh->HasNormals())
    {
        for(unsigned int j = 0; j < CurrentMesh->mNumVertices; j++)
        {
            vec3 Normal =
            {
                CurrentMesh->mNormals[j].x,
                CurrentMesh->mNormals[j].y,
                CurrentMesh->mNormals[j].z,
            };
            Result.Normals.push_back(Normal);
        }
    }

    // Extract Texture Coordinates
    if(CurrentMesh->HasTextureCoords(0)) // I'm not sure of the 0 parameter, what is this?
    {
        for(unsigned int j = 0; j < CurrentMesh->mNumVertices; j++)
        {
            vec2 TextureCoordinate =
            {
                CurrentMesh->mTextureCoords[0][j].x,
                CurrentMesh->mTextureCoords[0][j].y,
            };
            Result.TextureCoordinates.push_back(TextureCoordinate);
        }
    }

    // Extract Faces/Indices
    if(CurrentMesh->HasFaces())
    {
        for(unsigned int j = 0; j < CurrentMesh->mNumFaces; j++)
        {
            aiFace Face = CurrentMesh->mFaces[j];
            for(unsigned int k = 0; k < Face.mNumIndices; k++)
            {
                Result.Indices.push_back(Face.mIndices[k]);
            }
        }
    }

    // aiMaterial* Material = Scene->mMaterials[Mesh->mMaterialIndex];

    Meshes.push_back(Result);
}

void ProcessNode(const aiScene *Scene, const aiNode *Node)
{
    // Set the name for the model or mesh
    if(Node->mParent == NULL)
    {
        Model.Name = Node->mName.C_Str();
    }
    else
    {
        aiString Name = Node->mName;
    }

    // Extract meshes
    int MeshCount = Node->mNumMeshes;
    for(int i = 0; i < MeshCount; i++)
    {
        // Node->mMeshes is an array that contains indices to the Scene->mMeshes array
        ProcessMesh(Scene->mMeshes[Node->mMeshes[i]], Model.Meshes);
    }

    int ChildrenCount = Node->mNumChildren;
    for(int i = 0; i < ChildrenCount; i++)
    {
        ProcessNode(Scene, Node->mChildren[i]);
    }
}

int main(int Argc, char **Argv)
{
    Argc; Argv;

    Assimp::Importer Importer;
    // std::string Filename = "cube/cube.obj";
    // std::string Filename = "teapot.obj";
    std::string Filename = "survival/Survival_BackPack_2.fbx";

    const aiScene* Scene = Importer.ReadFile(Filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);
    if (Scene == nullptr)
    {
        std::cout << Importer.GetErrorString() << std::endl;
        return false;
    }

    // Let's process the scene, one node at a time
    ProcessNode(Scene, Scene->mRootNode);

    // TODO: Loop over the materials and store them
    if(Scene->HasMaterials())
    {
        aiMaterial *Material = Scene->mMaterials[1];

        aiString MaterialName;
        Material->Get(AI_MATKEY_NAME, MaterialName);
        printf("Material Name: %s\n", MaterialName.C_Str());

        aiString Str;
        Material->GetTexture(aiTextureType_DIFFUSE, 0, &Str);
        printf("Texture Name: %s\n", Str.C_Str());

        // Ambient Color
        aiColor3D AmbientColor;
        Material->Get(AI_MATKEY_COLOR_AMBIENT, AmbientColor);
        printf("Ambient Color: %2.2f\t%2.2f\t%2.2f\n", AmbientColor.r, AmbientColor.g, AmbientColor.b);

        // Diffuse Color
        aiColor3D DiffuseColor;
        Material->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
        printf("Diffuse Color: %2.2f\t%2.2f\t%2.2f\n", DiffuseColor.r, DiffuseColor.g, DiffuseColor.b);

        // Specular
        aiColor3D SpecularColor;
        Material->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor);
        printf("Specular Color: %2.2f\t%2.2f\t%2.2f\n", SpecularColor.r, SpecularColor.g, SpecularColor.b);

        int IlluminationModel;
        Material->Get(AI_MATKEY_SHADING_MODEL, IlluminationModel);
        printf("Illumination Model: %d\n", IlluminationModel);

        float SpecularExponent;
        Material->Get(AI_MATKEY_SHININESS, SpecularExponent);
        printf("Specular Exponent: %2.2f\n", SpecularExponent);
    }

    return 0;
}
