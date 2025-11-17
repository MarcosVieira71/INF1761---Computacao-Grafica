#version 410 core
layout(location = 0) in vec3 aPos;
uniform mat4 Mtex; 

void main()
{
    gl_Position = Mtex * vec4(aPos, 1.0);
}
