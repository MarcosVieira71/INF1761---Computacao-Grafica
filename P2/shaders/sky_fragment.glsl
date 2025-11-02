#version 410

in vec4 vColor;
in vec3 vTexCoord;


out vec4 outcolor;

uniform samplerCube skybox;

void main (void)
{
  outcolor = vColor * texture(skybox, vTexCoord);
}