#version 410

layout(location = 0) in vec3 coord;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec3 texcoord;

uniform mat4 Mv; 
uniform mat4 Mn; 
uniform mat4 Mvp;

out vec4 vColor;
out vec3 vTexCoord;

void main (void) 
{
  vTexCoord = coord;
  vColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  gl_Position = Mvp * vec4(coord, 1.0); 
}