#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Object.h"

using namespace std;

class Sphere : public Object
{
private:
    float radius;
    int sectors;
    int stacks;
    glm::vec3 center;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::ivec3> indicies;
    vector<glm::vec2> texCoords;

public:
    Sphere() : Object(glm::vec3(0.0f, 0.0f, 0.0f)){};
    Sphere(const glm::vec3 c, const float r, int sectorCount, int stackCount, const glm::vec3 col, float amb, float spec);
    ~Sphere(){};

    void setRadius(const float r) { radius = r; };
    float getRadius() { return radius; };

    void setCenter(const glm::vec3 &c) { center = c; };
    glm::vec3 getCenter() { return center; };

    vector<glm::vec3> getVertices() { return vertices; };
    vector<glm::vec3> getNormals() { return normals; };
    vector<glm::ivec3> getIndicies() { return indicies; };
    vector<glm::vec2> getTexCoords() { return texCoords; };
};

Sphere::Sphere(const glm::vec3 c,
               const float r,
               int sectorCount,
               int stackCount,
               const glm::vec3 col,
               float amb, 
               float spec)
{

    Object(col, amb, spec);

    vertices.resize(0);
    normals.resize(0);
    indicies.resize(0);
    texCoords.resize(0);

    sectors = sectorCount;
    stacks = stackCount;
    radius = r;
    center = c;

    glm::vec3 spherePosition;
    float xy;
    float sectorStep = 2.0f * M_PI / float(sectorCount);
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2.0f - i * stackStep;
        xy = radius * cosf(stackAngle);
        spherePosition.z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            /* Compute Positions */
            spherePosition.x = xy * cosf(sectorAngle);
            spherePosition.y = xy * sinf(sectorAngle);
            vertices.push_back(spherePosition);

            /* Compute Normals */
            normals.push_back(spherePosition / radius);

            /* Compute UV */
            texCoords.push_back(glm::vec2(((float)j / sectorCount), ((float)i / stackCount)));
        }
    }

    /* Compute Indicies */
    int k1, k2;

    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indicies.push_back(glm::ivec3(k1, k2, k1 + 1));
            }

            if (i != (stackCount - 1))
            {
                indicies.push_back(glm::ivec3(k1 + 1, k2, k2 + 1));
            }
        }
    }
}

#endif