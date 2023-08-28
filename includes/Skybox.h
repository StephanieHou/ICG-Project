#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "Image.h"

using namespace std;

class Skybox
{
private:
    GLuint mapTexture;

    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::ivec3> indicies;
    vector<glm::vec2> texCoords;

public:
    Skybox();
    ~Skybox(){};

    GLuint getTexture() { return mapTexture; }
    vector<glm::vec3> getVertices() { return vertices; }
    void createSkyBoxMap(vector<string> files);
};

Skybox::Skybox()
{
    vertices.resize(36);

    vertices[0] = glm::vec3(-500.0f, 500.0f, -500.0f);
    vertices[1] = glm::vec3(-500.0f, -500.0f, -500.0f);
    vertices[2] = glm::vec3(500.0f, -500.0f, -500.0f);
    vertices[3] = glm::vec3(500.0f, -500.0f, -500.0f);
    vertices[4] = glm::vec3(500.0f, 500.0f, -500.0f);
    vertices[5] = glm::vec3(-500.0f, 500.0f, -500.0f);

    vertices[6] = glm::vec3(-500.0f, -500.0f, 500.0f);
    vertices[7] = glm::vec3(-500.0f, -500.0f, -500.0f);
    vertices[8] = glm::vec3(-500.0f, 500.0f, -500.0f);
    vertices[9] = glm::vec3(-500.0f, 500.0f, -500.0f);
    vertices[10] = glm::vec3(-500.0f, 500.0f, 500.0f);
    vertices[11] = glm::vec3(-500.0f, -500.0f, 500.0f);

    vertices[12] = glm::vec3(500.0f, -500.0f, -500.0f);
    vertices[13] = glm::vec3(500.0f, -500.0f, 500.0f);
    vertices[14] = glm::vec3(500.0f, 500.0f, 500.0f);
    vertices[15] = glm::vec3(500.0f, 500.0f, 500.0f);
    vertices[16] = glm::vec3(500.0f, 500.0f, -500.0f);
    vertices[17] = glm::vec3(500.0f, -500.0f, -500.0f);

    vertices[18] = glm::vec3(-500.0f, -500.0f, 500.0f);
    vertices[19] = glm::vec3(-500.0f, 500.0f, 500.0f);
    vertices[20] = glm::vec3(500.0f, 500.0f, 500.0f);
    vertices[21] = glm::vec3(500.0f, 500.0f, 500.0f);
    vertices[22] = glm::vec3(500.0f, -500.0f, 500.0f);
    vertices[23] = glm::vec3(-500.0f, -500.0f, 500.0f);

    vertices[24] = glm::vec3(-500.0f, 500.0f, -500.0f);
    vertices[25] = glm::vec3(500.0f, 500.0f, -500.0f);
    vertices[26] = glm::vec3(500.0f, 500.0f, 500.0f);
    vertices[27] = glm::vec3(500.0f, 500.0f, 500.0f);
    vertices[28] = glm::vec3(-500.0f, 500.0f, 500.0f);
    vertices[29] = glm::vec3(-500.0f, 500.0f, -500.0f);

    vertices[30] = glm::vec3(-500.0f, -500.0f, -500.0f);
    vertices[31] = glm::vec3(-500.0f, -500.0f, 500.0f);
    vertices[32] = glm::vec3(500.0f, -500.0f, -500.0f);
    vertices[33] = glm::vec3(500.0f, -500.0f, -500.0f);
    vertices[34] = glm::vec3(-500.0f, -500.0f, 500.0f);
    vertices[35] = glm::vec3(500.0f, -500.0f, 500.0f);
}

void Skybox::createSkyBoxMap(vector<string> files)
{
    glGenTextures(1, &mapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mapTexture);

    for (int i = 0; i < files.size(); i++)
    {
        Image texture(files[i]);

        if (texture.getRawData())
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, stoi(texture.getWidth()), stoi(texture.getHeight()), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getRawData());
        }
        else
        {
            cout << "Error creating Skybox Map" << endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

#endif