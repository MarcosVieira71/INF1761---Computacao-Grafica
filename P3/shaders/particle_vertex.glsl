#version 410 core

layout(location = 0) in vec3 vertex;

uniform mat4 Mv;
uniform mat4 Mvp;

out vec4 vViewPos;
void main() {
     vViewPos = Mv * vec4(vertex, 1.0);
    gl_Position = Mvp * vec4(vertex, 1.0);
}
