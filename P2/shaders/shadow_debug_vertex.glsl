#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 3) in vec2 aTex;
out vec2 vTex;
void main() {
    vec2 pos = aPos * 2.0 - 1.0;
    gl_Position = vec4(pos, 0.0, 1.0);
    vTex = aTex;
}
