#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*
  Imported data is return in an aiScene structure.
  
  // Data things to know
  By default, assimp uses a left hand coordinate system the same as OpenGL. The output face winding is counter clockwise. Use #aiProcess_FlipWindingOrder to get CW data.

  // Texture things to know
  The output UV coordinate system has its origin in the lower-left corner. Use the #aiProcess_FlipUVs flag to get UV coordinates with the upper-left corner als origin.


  IMPORTANT: All meshes of an imported scene are stored in an array of aiMesh* inside the aiScene.
  IMPORTANT: materials are stored in an array inside aiScene

   One mesh uses only a single material everywhere - if parts of the model use a different material, this part is moved to a separate mesh at the same node
 */


//
// TODO: Define mesh struct. After that create a vector of meshes, extract everymesh onto this vector. Do the same for materials.
//

void ProcessNode(aiNode *Node)
{
    // The Root Node is mainly the file name. Children of root contain the actual meshes.
    aiString Name = Node->mName;
    printf("%s\n", Name.C_Str());

    // Extract meshes
    int MeshCount = Node->mNumMeshes;
    for(int i = 0; i < MeshCount; i++)
    {
        // Scene->mMeshes = *Node->mMeshes[i];
        printf("MeshIndex: %d", Node->mMeshes[i]);
    }

    int ChildrenCount = Node->mNumChildren;
    for(int i = 0; i < ChildrenCount; i++)
    {
        ProcessNode(Node->mChildren[i]);
    }
}

int main(int Argc, char **Argv)
{
    Argc; Argv;

    Assimp::Importer Importer;
    // std::string Filename = "cube/cube.obj";
    std::string Filename = "teapot/teapot.obj";

    const aiScene* Scene = Importer.ReadFile(Filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (Scene == nullptr)
    {
        std::cout << Importer.GetErrorString() << std::endl;
        return false;
    }

    // TODO: Extract UV coordinates
    // TODO: Extract vertex normals
    // TODO: Extract indices/faces
    // TODO: Extract Material | All materials are stored in an array of aiMaterial inside the aiScene.
    // TODO: Extract Material textures  | To test if it is an embedded texture use aiScene::GetEmbeddedTexture.

    int MeshCount = Scene->mNumMeshes;
    int MaterialCount = Scene->mNumMaterials;
    int AnimationCount = Scene->mNumAnimations;
    int TextureCount = Scene->mNumTextures;
    int LightCount = Scene->mNumLights;
    int CameraCount = Scene->mNumCameras;
    printf("MeshCount: %d\n", MeshCount);
    printf("MaterialCount: %d\n", MaterialCount);
    printf("AnimationCount: %d\n", AnimationCount);
    printf("TextureCount: %d\n", TextureCount);
    printf("LightCount: %d\n", LightCount);
    printf("CameraCount: %d\n", CameraCount);

    // TODO: Extract the vertices from each mesh, you can create obj files by hand to test things
    // TODO: We could try creating a global std::vector<mesh> and fill it in!
    // NOTE: We could extract everything first, vertices, materials, textures and store indicies in the mesh structs.
    // TODO: check aiMesh structure
    aiNode *RootNode = Scene->mRootNode;
    ProcessNode(RootNode);


    /*  THIS IS FROM LEARNOPENGL.COM model class
        // model data
        vector<Mesh> meshes;
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<textures> Textures;
     */

    return 0;
}
