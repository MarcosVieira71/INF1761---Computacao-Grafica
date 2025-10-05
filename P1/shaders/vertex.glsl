#version 410 core
layout(location = 0) in vec2 aPos;

uniform mat4 Mvp;
uniform mat4 Mv;

out vec3 vFragPos;
out vec3 vNormal;

void main() {
    vec4 posWorld = Mv * vec4(aPos,0.0,1.0);
    vFragPos = posWorld.xyz;

    vec3 n = vFragPos - (Mv * vec4(0,0,0,1)).xyz;
    if(length(n) < 1e-6) n = vec3(0,0,1);
    vNormal = normalize(n);

    gl_Position = Mvp * vec4(aPos,0.0,1.0);
}
