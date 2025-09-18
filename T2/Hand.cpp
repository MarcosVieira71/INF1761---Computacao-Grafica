#include "Hand.h"

#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include <glm/gtc/type_ptr.hpp>

Hand::Hand(float length, float width)
    : m_length(length), m_width(width)
{
    InitVertices();
    m_vertexNum = 3;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 3, m_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

HandPtr Hand::Make(float length, float width)
{
    return std::make_shared<Hand>(length, width);
}

Hand::~Hand()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Hand::InitVertices()
{
    m_vertices[0] = glm::vec2(0.0f, m_length);    
    m_vertices[1] = glm::vec2(-m_width / 2.0f, 0.0f); 
    m_vertices[2] = glm::vec2(m_width / 2.0f, 0.0f);  
}

void Hand::Draw() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexNum);
    glBindVertexArray(0);
}
