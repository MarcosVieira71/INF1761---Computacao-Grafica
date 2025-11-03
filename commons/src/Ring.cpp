#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Ring.h"
#include "Shader.h"
#include "State.h"

#include <glm/ext.hpp>
#include <vector>
#include <cmath>

Ring::Ring(float innerRadius, float outerRadius, std::size_t segments)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    for (size_t i = 0; i <= segments; ++i) {
        float angle = i * glm::two_pi<float>() / segments;
        float c = cos(angle);
        float s = sin(angle);

        vertices.push_back(glm::vec3(outerRadius * c, outerRadius * s, 0.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        texcoords.push_back(glm::vec2(0.5f + 0.5f * c, 0.5f + 0.5f * s));

        vertices.push_back(glm::vec3(innerRadius * c, innerRadius * s, 0.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        float rratio = innerRadius / outerRadius;
        texcoords.push_back(glm::vec2(0.5f + 0.5f * rratio * c, 0.5f + 0.5f * rratio * s));
    }

    m_vertexNum = static_cast<unsigned int>(vertices.size());

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_tbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

Ring::~Ring()
{
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_nbo) glDeleteBuffers(1, &m_nbo);
    if (m_tbo) glDeleteBuffers(1, &m_tbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

RingPtr Ring::Make(float innerRadius, float outerRadius, std::size_t segments)
{
    return RingPtr(new Ring(innerRadius, outerRadius, segments));
}

void Ring::Draw(StatePtr)
{
    GLboolean cullEnabled = glIsEnabled(GL_CULL_FACE);
    if (cullEnabled) glDisable(GL_CULL_FACE);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexNum);
    glBindVertexArray(0);

    if (cullEnabled) glEnable(GL_CULL_FACE);
}
