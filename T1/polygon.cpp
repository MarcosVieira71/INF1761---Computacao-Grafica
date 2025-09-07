#include "polygon.h"

#ifdef _WIN32
    #include <glad/glad.h>

#elif __APPLE__
    #include <OpenGL/gl3.h>

#else
    #include <GL/glew.h>
    #include <GL/gl.h>

#endif


Polygon::Polygon(const std::vector<glm::vec2>& vertices,
                 const std::vector<glm::vec3>& colors,
                const std::vector<unsigned int>& idxs) 
{    
    assert(vertices.size() == colors.size() && "vertices and colors must have the same size");
    m_vertexNum = vertices.size();
    m_idxs = idxs;

    setupVAO();

    glGenBuffers(2, m_vbo);
    glGenBuffers(1, &m_ebo);


    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,
                vertices.size() * sizeof(glm::vec2),
                vertices.data(),
                GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 colors.size() * sizeof(glm::vec3),
                 colors.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_idxs.size() * sizeof(unsigned int),
                 m_idxs.data(),
                 GL_STATIC_DRAW);
}

void Polygon::setupVAO()
{
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
}

PolygonPtr Polygon::Make(const std::vector<glm::vec2>& vertices, 
                        const std::vector<glm::vec3>& colors,
                        const std::vector<unsigned int>& idxs)
{
  return PolygonPtr(new Polygon(vertices, colors, idxs));
}

Polygon::~Polygon() 
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(2, m_vbo);

}

void Polygon::Draw() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_idxs.size(), GL_UNSIGNED_INT, 0);
}
