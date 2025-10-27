#ifdef _WIN32
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "Cylinder.h"
#include "Error.h"
#include "Grid.h"

#include <vector>
#include <glm/glm.hpp>
#include <cmath>

CylinderPtr Cylinder::Make(float radius, float height, int segments, bool caps)
{
  return CylinderPtr(new Cylinder(radius, height, segments, caps));
}

Cylinder::Cylinder(float radius, float height, int segments, bool caps)
{
  if (segments < 3) segments = 3;
  GridPtr grid = Grid::Make(segments, 1);
  const std::vector<float>& coords = grid->GetCoords(); 
  const std::vector<unsigned int>& inds = grid->GetIndices();

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texcoords;

  positions.reserve(inds.size());
  normals.reserve(inds.size());
  texcoords.reserve(inds.size());

  for (size_t k = 0; k < inds.size(); ++k) {
    unsigned int idx = inds[k];
    float u = coords[2 * idx + 0];
    float v = coords[2 * idx + 1];
    float ang = u * 2.0f * M_PI;
    float y = (v - 0.5f) * height;
    float x = radius * std::cos(ang);
    float z = radius * std::sin(ang);
    positions.emplace_back(x, y, z);
    glm::vec3 n = glm::normalize(glm::vec3(std::cos(ang), 0.0f, std::sin(ang)));
    normals.emplace_back(n);
    texcoords.emplace_back(u, v);
  }

  if (caps) {
    glm::vec3 topCenter(0.0f, height * 0.5f, 0.0f);
    glm::vec3 topNormal(0.0f, 1.0f, 0.0f);
    glm::vec2 centerTex(0.5f, 0.5f);
    for (int j = 0; j < segments; ++j) {
        int a = j;
        int b = (j + 1) % segments;
        float anga = a * 2.0f * M_PI / segments;
        float angb = b * 2.0f * M_PI / segments;
        glm::vec3 pa(radius * std::cos(anga),  height * 0.5f, radius * std::sin(anga));
        glm::vec3 pb(radius * std::cos(angb),  height * 0.5f, radius * std::sin(angb));
        glm::vec2 ta(0.5f + 0.5f * std::cos(anga), 0.5f + 0.5f * std::sin(anga));
        glm::vec2 tb(0.5f + 0.5f * std::cos(angb), 0.5f + 0.5f * std::sin(angb));
        positions.push_back(topCenter);
        positions.push_back(pa);
        positions.push_back(pb);
        normals.push_back(topNormal);
        normals.push_back(topNormal);
        normals.push_back(topNormal);
        texcoords.push_back(centerTex);
        texcoords.push_back(ta);
        texcoords.push_back(tb);
    }

    glm::vec3 bottomCenter(0.0f, -height * 0.5f, 0.0f);
    glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);
    for (int j = 0; j < segments; ++j) {
      int a = j;
      int b = (j + 1) % segments;
      float anga = a * 2.0f * M_PI / segments;
      float angb = b * 2.0f * M_PI / segments;
      glm::vec3 pa(radius * std::cos(anga), -height * 0.5f, radius * std::sin(anga));
      glm::vec3 pb(radius * std::cos(angb), -height * 0.5f, radius * std::sin(angb));
      glm::vec2 ta(0.5f + 0.5f * std::cos(anga), 0.5f + 0.5f * std::sin(anga));
      glm::vec2 tb(0.5f + 0.5f * std::cos(angb), 0.5f + 0.5f * std::sin(angb));
      positions.push_back(bottomCenter);
      positions.push_back(pa);
      positions.push_back(pb);
      normals.push_back(bottomNormal);
      normals.push_back(bottomNormal);
      normals.push_back(bottomNormal);
      texcoords.push_back(centerTex);
      texcoords.push_back(ta);
      texcoords.push_back(tb);
    }
  }

  m_vertexNum = static_cast<unsigned int>(positions.size());

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint id[3];
  glGenBuffers(3, id);

  glBindBuffer(GL_ARRAY_BUFFER, id[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, id[1]);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, id[2]);
  glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(3);

  m_vbo = id[0];
  m_nbo = id[1];
  m_tbo = id[2];

  glBindVertexArray(0);
}

Cylinder::~Cylinder()
{
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_nbo);
  glDeleteBuffers(1, &m_tbo);
  glDeleteVertexArrays(1, &m_vao);
}

void Cylinder::Draw(StatePtr)
{
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, m_vertexNum);
  glBindVertexArray(0);
}
