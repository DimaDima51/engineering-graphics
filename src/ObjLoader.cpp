#include "ObjLoader.h"
#include <cstdlib>
#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;

void loadObjModel(const string& modelPath, std::vector<Vertex>& resultVertexes){
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        modelPath.c_str(),
        aiProcess_Triangulate | aiProcess_GenNormals
    );

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE){
        printf("ERROR: ASSIMP\n");
        return;
    }

    aiMesh* mesh = scene->mMeshes[0];

    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];

        for(unsigned int j = 0; j < face.mNumIndices; j++){
            int idx = face.mIndices[j];

            aiVector3D pos = mesh->mVertices[idx];
            aiVector3D normal = mesh->mNormals[idx];
            aiVector3D tex = mesh->mTextureCoords[0][idx];

            resultVertexes.push_back(Vertex(
                vec3(pos.x, pos.y, pos.z),
                vec3(1.0f, 1.0f, 1.0f), // UPD: поменять цвета, если не забуду
                vec2(tex.x, tex.y),
                vec3(normal.x, normal.y, normal.z)
            ));
        }
    }
}