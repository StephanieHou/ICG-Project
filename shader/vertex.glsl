#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 mvpMatrix;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 norm;
out vec3 color;
out vec3 pos;
out vec2 tex;

void main()
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
    norm = mat3(transpose(inverse(modelMatrix))) * normal;
    color = vec3(0.5f, 0.5f, 0.5f);
    pos = vec3(modelMatrix * vec4(position, 1.0f));
    tex = texcoord;
}
