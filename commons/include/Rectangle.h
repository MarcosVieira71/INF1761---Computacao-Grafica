#pragma once
#include <memory>
#include "Shape.h"

class Rectangle;
using RectanglePtr = std::shared_ptr<Rectangle>;

class Rectangle : public Shape {
private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_tbo;
    unsigned int m_vertexNum;

protected:
    Rectangle(float width, float height);

public:
    static RectanglePtr Make(float width, float height);
    virtual ~Rectangle();
    virtual void Draw(StatePtr st) override;
};
