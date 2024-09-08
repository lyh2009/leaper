#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include "core/base.h"
#include "core/log.h"
#include "function/render/3d/mesh.h"
#include "function/render/texture.h"
#include "glm/fwd.hpp"
#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <meshoptimizer.h>
#include <string>
#include <vector>

Leaper::Model::Model(const std::string& path)
{
    LoadModel(path);
}

void Leaper::Model::Draw(glm::mat4& trans, glm::mat4& camera, glm::vec3 camera_pos, int entity_id)
{
    for (int i = 0; i < m_meshes.size(); ++i) { m_meshes[i].Render(trans, camera, camera_pos, entity_id); }
}

void Leaper::Model::LoadModel(const std::string& path)
{
    m_shader = Leaper::Shader::Create("./resource/shaders/3d/model.vert", "./resource/shaders/3d/model.frag");

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LP_LOG_ERROR(importer.GetErrorString());
        return;
    }

    m_path = path.substr(0, path.find_last_of('\\'));

    ProcessNode(scene->mRootNode, scene);
}

void Leaper::Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
        std::cout << node->mChildren[i]->mName.C_Str() << std::endl;
    }
}

Leaper::Mesh Leaper::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Leaper::Mesh::MeshVertex> vertices;
    std::vector<uint32_t> indices;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Leaper::Mesh::MeshVertex vertex;
        glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;

        vertex.position = vec;

        if (mesh->HasNormals())
        {
            vec.x         = mesh->mNormals[i].x;
            vec.y         = mesh->mNormals[i].y;
            vec.z         = mesh->mNormals[i].z;
            vertex.normal = vec;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texcoord;
            texcoord.x      = mesh->mTextureCoords[0][i].x;
            texcoord.y      = mesh->mTextureCoords[0][i].y;
            vertex.texcoord = texcoord;
        }
        else
            vertex.texcoord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j) { indices.push_back(face.mIndices[j]); }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Mesh::MeshTexture> textures;
    std::vector<Mesh::MeshTexture> diffuse_textures  = LoadTexture(material, aiTextureType_DIFFUSE, Leaper::TextureType::Diffuse);
    std::vector<Mesh::MeshTexture> specular_textures = LoadTexture(material, aiTextureType_SPECULAR, Leaper::TextureType::Specular);

    textures.insert(textures.begin(), diffuse_textures.begin(), diffuse_textures.end());
    textures.insert(textures.begin(), specular_textures.begin(), specular_textures.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Leaper::Mesh::MeshTexture> Leaper::Model::LoadTexture(aiMaterial* mat, aiTextureType type, Leaper::TextureType type_name)
{
    std::vector<Mesh::MeshTexture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = m_path + "\\" + std::string(str.C_Str());
        LP_CORE_LOG(path);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {  // 如果纹理还没有被加载，则加载它
            Mesh::MeshTexture texture;

            texture.texture = Leaper::Texture::Create(path, false);
            texture.type    = type_name;
            texture.path    = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // 添加到已加载的纹理中
        }
    }

    return textures;
}