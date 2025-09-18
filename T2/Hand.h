#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>

class Hand;
using HandPtr = std::shared_ptr<Hand>;

class Hand {
public:
    Hand(float length, float width);
    ~Hand();

    static HandPtr Make(float length, float width);

    void Draw() const;

private:
    void InitVertices();

    float m_length;
    float m_width;

    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_vertexNum;
    glm::vec2 m_vertices[3];

};
