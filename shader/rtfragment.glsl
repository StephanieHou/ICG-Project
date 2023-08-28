#version 150 core

out vec4 rtFragColor;

in vec2 quadTex;

uniform sampler2D rtTextureID;
uniform sampler2D bTextureID;
uniform float gammaCorrection;

void main()
{  
    vec3 rtColor = texture(rtTextureID, quadTex).rgb;      
    vec3 bColor = texture(bTextureID, quadTex).rgb;

    rtColor += bColor;

    vec3 result = vec3(1.0f) - exp(-rtColor * 2.0f);  

    result = pow(result, vec3(1.0f / gammaCorrection));

    rtFragColor = vec4(result, 1.0f);
}
