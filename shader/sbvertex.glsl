#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 mvpMatrix;

in vec3 position;

out vec3 tex;

void main()
{
    vec4 pos = projMatrix * viewMatrix * vec4(position, 1.0f);
    tex = position;
    gl_Position = pos.xyww;
}
