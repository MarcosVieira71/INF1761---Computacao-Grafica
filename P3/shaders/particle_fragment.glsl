#version 410 core

in vec4 vColor;
out vec4 outcolor;

void main() {
    outcolor = vec4(vColor.rgb, 0.9);
}
