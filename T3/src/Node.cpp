#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Node.h"

#include "Appearance.h"
#include "Error.h"
#include "Shader.h"
#include "Shape.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>


#include <iostream>

Node::Builder& Node::Builder::WithShader(ShaderPtr s) { 
    shader = s; 
    return *this; 
}

Node::Builder& Node::Builder::WithTransform(TransformPtr t) { 
    trf = t; 
    return *this; 
}

Node::Builder& Node::Builder::AddAppearance(AppearancePtr a) { 
    apps.push_back(a); 
    return *this; 
}

Node::Builder& Node::Builder::AddShape(ShapePtr s) { 
    shps.push_back(s); 
    return *this; 
}

Node::Builder& Node::Builder::AddNode(NodePtr n) { 
    nodes.push_back(n); 
    return *this; 
}

NodePtr Node::Builder::Build() {
    auto n = std::shared_ptr<Node>(new Node());
    n->m_shader = shader;
    n->m_trf = trf;
    n->m_apps = std::move(apps);
    n->m_shps = std::move(shps);
    n->m_nodes = std::move(nodes);

    for (auto& child : n->m_nodes) {
        if (child) child->SetParent(n);
    }
    return n;
}

void Node::SetShader(ShaderPtr shader) {
    m_shader = shader;
}

void Node::AddShape(ShapePtr shp) {
    m_shps.push_back(shp);
}

void Node::AddNode(NodePtr node) {
    m_nodes.push_back(node);
    if (node) node->SetParent(shared_from_this());
}

void Node::SetParent(NodePtr parent) {
    m_parent = parent;
}

void Node::SetTransform(TransformPtr trf) {
    m_trf = trf;
}

void Node::AddAppearance(AppearancePtr app) {
    m_apps.push_back(app);
}

glm::mat4 Node::GetMatrix () const
{
  return m_trf ? m_trf->GetMatrix() : glm::mat4(1.0f);
}

glm::mat4 Node::GetModelMatrix () 
{
  glm::mat4 mat = GetMatrix();
  NodePtr node = GetParent();
  while (node != nullptr) {
    mat = node->GetMatrix() * mat;
    node = node->GetParent();
  } 
  return mat;
}

NodePtr Node::GetParent() const {
    return m_parent.lock();
}

void Node::Render(StatePtr st) {
    if (m_shader) m_shader->Load(st);
    if (m_trf) m_trf->Load(st);

    for (AppearancePtr app : m_apps)
        app->Load(st);

    if (!m_shps.empty()) {
        st->LoadMatrices();
        for (ShapePtr shp : m_shps)
            shp->Draw(st);
    }

    for (NodePtr node : m_nodes)
        node->Render(st);

    for (AppearancePtr app : m_apps)
        app->Unload(st);

    if (m_trf) m_trf->Unload(st);
    if (m_shader) m_shader->Unload(st);

    Error::Check("end node render");
}




