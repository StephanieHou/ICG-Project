#version 150 core

out vec4 fragColor;
out vec4 brightFragColor;

in vec3 norm;
in vec3 color;
in vec3 pos;
in vec2 tex;

uniform vec3 camPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float ambient;
uniform int bloom;
uniform int isEarth;

uniform sampler2D textureID;
uniform sampler2D specTextureID;

void main()
{
    vec3 normal = normalize(norm);
    vec4 textureColor = texture(textureID, tex);
    vec3 textureRGB = textureColor.rgb;

    vec3 viewDirection = normalize(camPosition - pos);
    vec3 lightDirection = normalize(lightPosition - pos);
    vec3 reflectDirection = reflect(-lightDirection, normal); 

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    float shineness = 32.0f;
    float strength = 0.8f;
    float diffuse = max(dot(lightDirection, normal), 0.0f);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), shineness) * strength;

    if(bloom > 0){
        result = lightColor * (diffuse + ambient) * textureRGB;

        brightFragColor = vec4(result, 1.0f);
    }
    else{

        vec3 f = 100.0f / (100.0f + vec3(33.f, 13.f, 9.f));
        vec3 on = vec3(1.0f, 0.0f, 0.0f) + vec3(-0.5f, 0.17f, 0.45f) * f;

        float cosnl = clamp(dot(normal, lightDirection), 0.0f, 1.0f);
        float cosnv = clamp(dot(normal, viewDirection), 0.0f, 1.0f);
        float ons = clamp(dot(lightDirection, viewDirection), 0.0f, 1.0f) - cosnl * cosnv;
        ons /= mix(max(cosnl, cosnv), 1.0f, step(ons, 0.0f));

        result = textureRGB * cosnl * (on.x + textureRGB * on.y + on.z * ons);

        brightFragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    if(isEarth > 0){
        vec3 specColor = texture(specTextureID, tex).rgb;

        if(specColor == vec3(1.0f, 1.0f, 1.0f)){
            result = lightColor * (ambient + diffuse + specular) * textureRGB;
        }
    }

    fragColor = vec4(result, 1.0f);
}
