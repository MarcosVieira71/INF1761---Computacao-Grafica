#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec2 texcoord;

uniform mat4 Mvp;
uniform mat4 Mv;
uniform mat4 Mn;
uniform mat4 Mtex;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vShadowCoord;

void main() {
    vec4 worldPos = Mv * vec4(vertex, 1.0);
    vFragPos = worldPos.xyz;
    vNormal = normalize((Mn * vec4(normal, 0.0)).xyz);
    vTexCoord = texcoord;
    vShadowCoord = Mtex * vec4(vFragPos, 1.0);
    gl_Position = Mvp * vec4(vertex, 1.0);
}
