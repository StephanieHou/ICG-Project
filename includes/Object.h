#ifndef OBJECT_H_
#define OBJECT_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

using namespace std;

class Object
{
private:
    glm::vec3 color;
    float ambient;
    float specularExp;

public:
    Object(){};
    Object(glm::vec3 col, float amb = 0.8f, float spec = 50.0f)
    {
        color = col;
        ambient = amb;
        specularExp = spec;
    };

    virtual ~Object(){};

    const glm::vec3 getColor() { return color; };
    void setColor(glm::vec3 col) { color = col; }

    float getAmbient() { return ambient; };
    void setAmbient(float amb) { ambient = amb; }

    float getSpecularExp() { return specularExp; };
    void setSpecularExp(float spec) { specularExp = spec; }
};

#endif