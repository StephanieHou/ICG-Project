#version 150 core

out vec4 bFragColor;

in vec2 quadTex;

uniform sampler2D bTextureID;
uniform bool curBFBO;

uniform float weights[5] = float[] (0.227f, 0.194f, 0.121f, 0.054f, 0.016f);


void main()
{

     vec2 offset = 1.0f / textureSize(bTextureID, 0);
     vec3 result = texture(bTextureID, quadTex).rgb * weights[0];

     if(curBFBO)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(bTextureID, quadTex + vec2(offset.x * i * 4.0f, 0.0f)).rgb * weights[i];
            result += texture(bTextureID, quadTex - vec2(offset.x * i * 4.0f, 0.0f)).rgb * weights[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(bTextureID, quadTex + vec2(0.0f, offset.y * i * 4.0f)).rgb * weights[i];
             result += texture(bTextureID, quadTex - vec2(0.0f, offset.y * i * 4.0f)).rgb * weights[i];
         }
     }

    bFragColor = vec4(result, 1.0f);
}
