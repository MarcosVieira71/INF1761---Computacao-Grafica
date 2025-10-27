#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Sphere.h"
#include "Grid.h"
#include "Error.h"

#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.14159265f

SpherePtr Sphere::Make (int nstack, int nslice)
{
  return SpherePtr(new Sphere(nstack,nslice));
}


Sphere::Sphere(int nstack, int nslice)
{
    GridPtr grid = Grid::Make(nstack, nslice);
    m_nind = grid->IndexCount();

    const size_t vertexCount = grid->VertexCount();
    auto texcoord = grid->GetCoords();

    // Alocação automática e segura
    std::vector<float> coord(3 * (nstack + 1) * (nslice + 1));
    std::vector<float> tangent(3 * (nstack + 1) * (nslice + 1));

    size_t nc = 0;
    for (int i = 0; i < 2 * vertexCount; i += 2) {
        float theta = texcoord[i + 0] * 2.0f * M_PI;
        float phi   = texcoord[i + 1] * M_PI;

        coord[nc + 0] = std::sin(theta) * std::sin(M_PI - phi);
        coord[nc + 1] = std::cos(M_PI - phi);
        coord[nc + 2] = std::cos(theta) * std::sin(M_PI - phi);

        tangent[nc + 0] = std::cos(theta);
        tangent[nc + 1] = 0.0f;
        tangent[nc + 2] = -std::sin(theta);

        nc += 3;
    }

    // === Criação do VAO ===
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint id[3];  // coord/normal, tangent, texcoord
    glGenBuffers(3, id);

    // Buffer de coordenadas (posição + normal)
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, coord.size() * sizeof(float), coord.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Buffer de tangentes
    glBindBuffer(GL_ARRAY_BUFFER, id[1]);
    glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(float), tangent.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // Buffer de coordenadas de textura
    glBindBuffer(GL_ARRAY_BUFFER, id[2]);
    glBufferData(GL_ARRAY_BUFFER, 2 * vertexCount * sizeof(float), texcoord.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    // Índices
    GLuint index;
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nind * sizeof(unsigned int), grid->GetIndices().data(), GL_STATIC_DRAW);
}

Sphere::~Sphere () 
{
}

void Sphere::Draw (StatePtr st)
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES,m_nind,GL_UNSIGNED_INT,0);
}