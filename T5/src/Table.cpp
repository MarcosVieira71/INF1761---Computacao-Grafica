#include "Table.h"

#include "Color.h"
#include "Cube.h"
#include "Transform.h"


Table::Leg::LegPtr Table::Leg::Make(const glm::vec3& pos, float height)
{
    return std::make_shared<Leg>(pos, height);
}

Table::Leg::~Leg() = default;

Table::Leg::Leg(const glm::vec3& relativePos, float height)
{
    auto trfCube = Transform::Make();

    trfCube->Translate(relativePos.x, relativePos.y, relativePos.z);
    trfCube->Scale(0.1f, height, 0.1f);

    AddShape(Cube::Make());
    AddAppearance(Color::Make(0.8f, 0.5f, 0.3f));
    SetTransform(trfCube);
}



Table::Table(const glm::vec3& pos)
{
    auto trfCube = Transform::Make();
    trfCube->Translate(pos.x, pos.y, pos.z);
    trfCube->Scale(5.0f, 0.2f, 5.0f);

    AddShape(Cube::Make());
    AddAppearance(Color::Make(1.0f, 0.3f, 0.3f));
    SetTransform(trfCube);
}

TablePtr Table::Make(const glm::vec3& pos)
{
    return std::make_shared<Table>(pos);
}

void Table::setupLegs()
{
    AddNode(Table::Leg::Make(glm::vec3(0.4, -20, 0.4), 20));
    AddNode(Table::Leg::Make(glm::vec3(-0.4, -20, 0.4), 20));
    AddNode(Table::Leg::Make(glm::vec3(-0.4, -20, -0.4), 20));
    AddNode(Table::Leg::Make(glm::vec3(0.4, -20, -0.4), 20));
}




Table::~Table() = default;

