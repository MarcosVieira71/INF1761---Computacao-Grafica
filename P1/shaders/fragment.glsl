#version 410 core
in vec3 vFragPos;
in vec3 vNormal;

out vec4 outcolor;

uniform vec4 color; // cor do objeto
uniform vec4 lpos;  // posição do Sol

void main() {
    vec3 lightDir = normalize(lpos.xyz - vFragPos); 
    float diff = max(dot(vNormal, lightDir), 0.0);
    vec3 result = color.rgb * diff;

    outcolor = color;
}
