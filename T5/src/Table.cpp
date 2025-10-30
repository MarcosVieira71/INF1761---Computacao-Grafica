#include "Table.h"

#include "Base.h"
#include "Color.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Transform.h"


Table::Leg::LegPtr Table::Leg::Make(const glm::vec3& pos, float height, ShapePtr s)
{
    return std::make_shared<Leg>(pos, height, s);
}

Table::Leg::~Leg() = default;

Table::Leg::Leg(const glm::vec3& relativePos, float height, ShapePtr s)
{
    auto trfLegs = Transform::Make();

    trfLegs->Translate(relativePos.x, relativePos.y, relativePos.z);
    trfLegs->Scale(0.5f, height, 0.5f);

    AddShape(s);
    SetTransform(trfLegs);
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


Table::Table(const glm::vec3& pos, AppearancePtr app) : _pos(pos)
{
    auto trfTable = Transform::Make();
    trfTable->Translate(pos.x, pos.y, pos.z);
    
    AddAppearance(app);
    SetTransform(trfTable);
}

TablePtr Table::Make(const glm::vec3& pos, AppearancePtr app, AppearancePtr legs)
{
    auto table = std::make_shared<Table>(pos, app);
    table->AddNode(Table::Top::Make({5.0f, 0.2f, 5.0f}));
    if(!legs) legs = app;
    auto nLegsApp = Node::Builder().AddAppearance(legs).Build();
    table->AddNode(nLegsApp);
    ShapePtr s = Cylinder::Make(1,1,64);
    nLegsApp->AddNode(Table::Leg::Make(glm::vec3(2.0, -2.5f, 2.0), 5.0f, s));
    nLegsApp->AddNode(Table::Leg::Make(glm::vec3(-2.0, -2.5f, 2.0), 5.0f, s));
    nLegsApp->AddNode(Table::Leg::Make(glm::vec3(-2.0, -2.5f, -2.0), 5.0f, s));
    nLegsApp->AddNode(Table::Leg::Make(glm::vec3(2.0, -2.5f, -2.0), 5.0f, s));
    return table;
}

void Table::setup(BasePtr base)
{
    AddNode(base);
}




Table::~Table() = default;

