#version 330 core
in vec2 vTex;
out vec4 FragColor;
uniform sampler2D shadowMap;
void main() {
    float d = texture(shadowMap, vTex).r;
    FragColor = vec4(vec3(d), 1.0);
}
