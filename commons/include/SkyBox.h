#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shape.h"
#include "State.h"


class SkyBox;
using SkyBoxPtr = std::shared_ptr<SkyBox>;

class SkyBox : public Shape
{
    
public:
    SkyBox();
    static SkyBoxPtr Make();
    virtual ~SkyBox() = default;

    void Draw(StatePtr st) override;

private:
    unsigned m_vao;
    unsigned m_vbo;
};
