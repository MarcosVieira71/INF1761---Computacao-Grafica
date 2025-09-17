#version 410 core

layout(location = 0) in vec2 vertex;

uniform mat4 proj;
uniform mat4 model;
uniform vec3 uniformColor;

out vec3 fragColor;

void main()
{
    gl_Position = proj * model * vec4(vertex, 0.0, 1.0);
    fragColor = uniformColor;
}
