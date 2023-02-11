#include "simple_mesh.hpp"

SimpleMeshData concatenate(SimpleMeshData aM, SimpleMeshData const& aN)
{
    aM.positions.insert(aM.positions.end(), aN.positions.begin(), aN.positions.end());
    aM.colors.insert(aM.colors.end(), aN.colors.begin(), aN.colors.end());
    aM.normals.insert(aM.normals.end(), aN.normals.begin(), aN.normals.end());
    aM.texcoords.insert(aM.texcoords.end(), aN.texcoords.begin(), aN.texcoords.end());
    return aM;
}

SimpleMeshData concatenate(int count, ...)
{
    va_list meshList;
    va_start(meshList, count);
    SimpleMeshData aM;
    for (int i = 0; i < count; i++) {
        SimpleMeshData mesh = va_arg(meshList, SimpleMeshData);
        aM.positions.insert(aM.positions.end(), mesh.positions.begin(), mesh.positions.end());
        aM.colors.insert(aM.colors.end(), mesh.colors.begin(), mesh.colors.end());
    }
    return aM;
}


GLuint create_vao(SimpleMeshData const& aMeshData)
{
    // position VBO
    GLuint positionVBO = 0;
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW);

    // normal VBO
    GLuint normalVBO = 0;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

    // color VBO
    GLuint colorVBO = 0;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW);


    // create the VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    // add position
    glVertexAttribPointer(
        0, // location = 0 in vertex shader
        3, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to [0, 1]
        3 * sizeof(GLfloat), // stride = 0, no padding between inputs
        0 // data starts at offset 0 in the VBO
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    // add normal
    glVertexAttribPointer(
        1, // location = 1 in vertex shader
        3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized
        3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    // add color
    glVertexAttribPointer(
        2, // location = 2 in vertex shader
        3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized
        3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(2);

    return vao;
}


std::vector<GLuint> create_vao(SimpleMeshData const& aMeshData, std::string texturePath, std::string specularPath)
{
    // create the VAO
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    std::vector<GLuint> vector;
    // position VBO
    GLuint positionVBO = 0;
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW);

    // normal VBO
    GLuint normalVBO = 0;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

    // color VBO
    GLuint colorVBO = 0;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW);

    // texture VBO
    GLuint textureVBO = 0;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.texcoords.size() * sizeof(Vec2f), aMeshData.texcoords.data(), GL_STATIC_DRAW);

    // texture
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image data
    int width, height, channelsNum;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(texturePath.c_str(), &width, &height, &channelsNum, 0);
    if (image) {
        if (texturePath.substr(texturePath.length() - 3, 3) == "png") {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        }
        else if (texturePath.substr(texturePath.length() - 3, 3) == "jpg") {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(image);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    // add texture
    glVertexAttribPointer(
        3, // location = 3 in vertex shader
        2, GL_FLOAT, GL_FALSE, // 2 floats, not normalized
        2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(3);


    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    // add position
    glVertexAttribPointer(
        0, // location = 0 in vertex shader
        3, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to [0, 1]
        3 * sizeof(GLfloat), // stride = 0, no padding between inputs
        0 // data starts at offset 0 in the VBO
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    // add normal
    glVertexAttribPointer(
        1, // location = 1 in vertex shader
        3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized
        3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    // add color
    glVertexAttribPointer(
        2, // location = 2 in vertex shader
        3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized
        3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(2);

    vector.emplace_back(vao);
    vector.emplace_back(texture);

    if(specularPath != ""){
        // specular map
        GLuint specularMap = 0;
        glGenTextures(1, &specularMap);

        glBindTexture(GL_TEXTURE_2D, specularMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load data
        stbi_set_flip_vertically_on_load(true);
        image = stbi_load(specularPath.c_str(), &width, &height, &channelsNum, 0);
        if (image) {
            if (specularPath.substr(specularPath.length() - 3, 3) == "png") {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            }
            else if (specularPath.substr(specularPath.length() - 3, 3) == "jpg") {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(image);
        vector.emplace_back(specularMap);
    }

    return vector;
}