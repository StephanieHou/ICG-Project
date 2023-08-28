#version 150 core

out vec4 sbFragColor;

in vec3 tex;

uniform samplerCube textureID;

void main()
{
    sbFragColor = texture(textureID, tex);
}
