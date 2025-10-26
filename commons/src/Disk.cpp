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
    std::vector<glm::vec2> texCoords;

    vertices.push_back(glm::vec2(0.0f)); // centro na origem
    texCoords.push_back(glm::vec2(0.5f, 0.5f)); // centro da textura


    for (size_t i = 0; i <= segments; i++) {
        float angle = i * glm::two_pi<float>() / segments;

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        vertices.push_back(glm::vec2(x, y));

        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);
        texCoords.push_back(glm::vec2(u, v));

    }

    m_vertexNum = static_cast<unsigned int>(vertices.size());

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_tbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glEnableVertexAttribArray(3);

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
