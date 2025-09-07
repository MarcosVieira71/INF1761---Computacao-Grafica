#include "polygon.h"

#include <iostream>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

PolygonPtr Polygon::Make ()
{
  return PolygonPtr(new Polygon());
}

Polygon::Polygon ()
{
  float coord[] = {
    -0.5f,-0.5f, 
     0.5f,-0.5f,
     0.0f, 0.5f,
  };
  unsigned char color[] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255
  };
  
  // create VAO
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  // create coord buffer
  
  GLuint id[2];
  glGenBuffers(2,id);
  glBindBuffer(GL_ARRAY_BUFFER,id[0]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(coord),coord,GL_STATIC_DRAW);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);  // coord
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,id[1]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_UNSIGNED_BYTE,GL_TRUE,0,0);  // color
  glEnableVertexAttribArray(1);
}

Polygon::~Polygon () 
{
}

void Polygon::Draw ()
{
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES,0,3);
}