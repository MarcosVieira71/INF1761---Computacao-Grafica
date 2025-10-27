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
    trfCube->Scale(0.5f, height, 0.5f);

    AddShape(Cube::Make());
    SetTransform(trfCube);
}

Table::Top::TopPtr Table::Top::Make(const glm::vec3& scale)
{
    return std::make_shared<Top>(scale);
}

Table::Top::~Top() = default;

Table::Top::Top(const glm::vec3& scale)
{
    auto trfCube = Transform::Make();
    trfCube->Scale(scale.x, scale.y, scale.z);
    AddShape(Cube::Make());
    SetTransform(trfCube);
}


Table::Table(const glm::vec3& pos) : _pos(pos)
{
    auto trfTable = Transform::Make();
    trfTable->Translate(pos.x, pos.y, pos.z);
    
    // AddShape(Cube::Make());
    AddAppearance(Color::Make(1.0f, 0.3f, 0.3f));
    SetTransform(trfTable);
}

TablePtr Table::Make(const glm::vec3& pos)
{
    return std::make_shared<Table>(pos);
}

void Table::setup()
{
    AddNode(Table::Top::Make({5.0f, 0.2f, 5.0f}));
    AddNode(Table::Leg::Make(glm::vec3(2.0, -20, 2.0), 20));
    AddNode(Table::Leg::Make(glm::vec3(-2.0, -20, 2.0), 20));
    AddNode(Table::Leg::Make(glm::vec3(-2.0, -20, -2.0), 20));
    AddNode(Table::Leg::Make(glm::vec3(2.0, -20, -2.0), 20));
}




Table::~Table() = default;

