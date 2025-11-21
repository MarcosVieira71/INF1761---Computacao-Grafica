#version 410 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 outcolor;

uniform vec4 color;
uniform vec4 lpos;
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;
uniform vec4 cpos;
uniform sampler2D decal;  
uniform int hasTexture;
uniform int isEmissive;
uniform vec3 emissionColor;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lpos.xyz - vFragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 V = normalize(cpos.xyz - vFragPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), 32.0);

    float distance = length(lpos.xyz - vFragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec3 ambient = lamb.rgb * color.rgb;
    vec3 diffuse = ldif.rgb * color.rgb * diff;
    vec3 specular = lspe.rgb * spec;

    vec3 lighting = (ambient + diffuse + specular) * attenuation;
    if (isEmissive == 1) lighting += emissionColor;
    vec3 result;
    float alpha;

    vec4 texColor = texture(decal, vTexCoord);
    result = lighting * texColor.rgb;
    alpha = color.a;


    outcolor = vec4(result, alpha);
}
