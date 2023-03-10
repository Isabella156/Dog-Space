#ifndef SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
#define SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9

#include <glad.h>
#include <string>

#include <vector>
#include <stdarg.h>
#include "../vmlib/vec3.hpp"
#include "../vmlib/vec2.hpp"
#include "../third_party/stb/include/stb_image.h"

struct SimpleMeshData
{
    std::vector<Vec3f> positions;
    std::vector<Vec3f> colors;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> texcoords;
};

SimpleMeshData concatenate(SimpleMeshData, SimpleMeshData const&);
SimpleMeshData concatenate(int count, ...);


GLuint create_vao(SimpleMeshData const&);
std::vector<GLuint> create_vao(SimpleMeshData const& aMeshData, std::string texturePath, std::string specularMap);
#endif // SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
