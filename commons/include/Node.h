#include <memory>
#pragma once

class Node;
using NodePtr = std::shared_ptr<Node>; 


#include <glm/glm.hpp>
#include <vector>

class Appearance;
class Shape;
class Shader;
class Transform;
class Node;
class State;

using AppearancePtr = std::shared_ptr<Appearance>;
using ShapePtr      = std::shared_ptr<Shape>;
using ShaderPtr     = std::shared_ptr<Shader>;
using TransformPtr  = std::shared_ptr<Transform>;
using NodePtr       = std::shared_ptr<Node>;
using StatePtr      = std::shared_ptr<State>;

class Node : public std::enable_shared_from_this<Node>
{
    std::weak_ptr<Node> m_parent;       
    ShaderPtr m_shader;                 
    TransformPtr m_trf;                 
    std::vector<AppearancePtr> m_apps;  
    std::vector<ShapePtr> m_shps;       
    std::vector<NodePtr> m_nodes;          
protected:
    Node() = default;

public:
    virtual ~Node() = default;

    class Builder {
        ShaderPtr shader;
        TransformPtr trf;
        std::vector<AppearancePtr> apps;
        std::vector<ShapePtr> shps;
        std::vector<NodePtr> nodes;

    public:
        Builder& WithShader(ShaderPtr s);
        Builder& WithTransform(TransformPtr t);
        Builder& AddAppearance(AppearancePtr a);
        Builder& AddShape(ShapePtr s);
        Builder& AddNode(NodePtr n);

        NodePtr Build();
    };

    void SetShader(ShaderPtr shader);
    void SetTransform(TransformPtr trf);
    void AddAppearance(AppearancePtr app);
    void AddShape(ShapePtr shp);
    void AddNode(NodePtr node);
    void SetParent(NodePtr parent);
    NodePtr GetParent() const;
    glm::mat4 GetMatrix() const;
    glm::mat4 GetModelMatrix();
    TransformPtr& GetTransform();
    void Render(StatePtr st);
};
