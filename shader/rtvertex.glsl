#version 150 core

in vec3 quadPosition;

out vec2 quadTex;

void main()
{
    gl_Position = vec4(quadPosition, 1.0f);
    quadTex = (quadPosition.xy + vec2(1.0f,1.0f))/2.0f;
}
