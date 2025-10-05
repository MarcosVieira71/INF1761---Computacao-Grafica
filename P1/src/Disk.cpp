#include "Disk.h"


#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Shader.h"
#include "State.h"
#include "Camera2D.h"

#include <glm/ext.hpp>
#include <cmath>
#include <vector>

Disk::Disk(float radius, std::size_t segments)
{
    std::vector<glm::vec2> vertices;
    vertices.push_back(glm::vec2(0.0f)); // centro na origem

    for (size_t i = 0; i <= segments; i++) {
        float angle = i * glm::two_pi<float>() / segments;
        vertices.push_back(glm::vec2(
            radius * cos(angle),
            radius * sin(angle)
        ));
    }

    m_vertexNum = static_cast<GLsizei>(vertices.size());

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Disk::~Disk()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

DiskPtr Disk::Make(float radius, std::size_t segments)
{
    return DiskPtr(new Disk(radius, segments));
}

void Disk::Draw(StatePtr) 
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertexNum);
    glBindVertexArray(0);
}
