#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Obj.h"
#include "Error.h"

#include "Node.h"
#include "Texture.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

Obj::Obj()
  : m_vao(0), m_count(0)
{
  m_vbo[0] = m_vbo[1] = m_vbo[2] = 0;
}

static void split_face_token(const std::string& tok, int& vi, int& ti, int& ni)
{
  vi = ti = ni = 0;
  std::stringstream ss(tok);
  std::string part;
  std::getline(ss, part, '/');
  if (!part.empty()) vi = std::stoi(part);
  if (ss.good()) {
    std::getline(ss, part, '/');
    if (!part.empty()) ti = std::stoi(part);
    if (ss.good()) {
      std::getline(ss, part, '/');
      if (!part.empty()) ni = std::stoi(part);
    }
  }
}

ObjPtr Obj::MakeFromFile(const std::string& filename)
{
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "Obj: cannot open file: " << filename << "\n";
    return nullptr;
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texcoords;

  std::vector<float> out_coords;
  std::vector<float> out_normals;
  std::vector<float> out_texcoords;

  std::string line;
  while (std::getline(in, line)) {
    if (line.size() < 2) continue;
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;
    if (tag == "v") {
      float x,y,z; iss >> x >> y >> z; positions.emplace_back(x,y,z);
    } else if (tag == "vn") {
      float x,y,z; iss >> x >> y >> z; normals.emplace_back(x,y,z);
    } else if (tag == "vt") {
      float u,v; iss >> u >> v; texcoords.emplace_back(u,v);
    } else if (tag == "f") {
      std::vector<std::string> tokens;
      std::string tok;
      while (iss >> tok) tokens.push_back(tok);
      for (size_t i = 1; i + 1 < tokens.size(); ++i) {
        std::string t0 = tokens[0];
        std::string t1 = tokens[i];
        std::string t2 = tokens[i+1];
        std::string tri[3] = {t0, t1, t2};
        for (int k=0;k<3;++k) {
          int vi=0, ti=0, ni=0;
          split_face_token(tri[k], vi, ti, ni);
          if (vi!=0) {
            glm::vec3 p = positions[ (vi>0?vi-1:positions.size()+vi) ];
            out_coords.push_back(p.x);
            out_coords.push_back(p.y);
            out_coords.push_back(p.z);
          } else {
            out_coords.push_back(0.0f); out_coords.push_back(0.0f); out_coords.push_back(0.0f);
          }
          if (ni!=0) {
            glm::vec3 n = normals[ (ni>0?ni-1:normals.size()+ni) ];
            out_normals.push_back(n.x);
            out_normals.push_back(n.y);
            out_normals.push_back(n.z);
          } else {
            out_normals.push_back(0.0f); out_normals.push_back(0.0f); out_normals.push_back(0.0f);
          }
          if (ti!=0) {
            glm::vec2 uv = texcoords[ (ti>0?ti-1:texcoords.size()+ti) ];
            out_texcoords.push_back(uv.x);
            out_texcoords.push_back(uv.y);
          } else {
            out_texcoords.push_back(0.0f); out_texcoords.push_back(0.0f);
          }
        }
      }
    }
  }

  if (out_coords.empty()) {
    std::cerr << "Obj: no geometry loaded from: " << filename << "\n";
    return nullptr;
  }

  ObjPtr obj(new Obj());

  obj->m_count = (unsigned int)(out_coords.size() / 3);

  glGenVertexArrays(1, &obj->m_vao);
  glBindVertexArray(obj->m_vao);

  GLuint vbo[3];
  glGenBuffers(3, vbo);
  obj->m_vbo[0] = vbo[0]; obj->m_vbo[1] = vbo[1]; obj->m_vbo[2] = vbo[2];

  // coords
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, out_coords.size()*sizeof(float), out_coords.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, out_normals.size()*sizeof(float), out_normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  // texcoords
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, out_texcoords.size()*sizeof(float), out_texcoords.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(3);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Error::Check("Obj::MakeFromFile - after GL setup");

  return obj;
}

Obj::~Obj()
{
  if (m_vbo[0] || m_vbo[1] || m_vbo[2]) {
    GLuint ids[3] = { m_vbo[0], m_vbo[1], m_vbo[2] };
    glDeleteBuffers(3, ids);
  }
  if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void Obj::Draw(StatePtr)
{
  if (!m_vao) return;
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, m_count);
}

static std::string find_texture_in_mtl(const std::string& mtlpath, const std::string& matname)
{
  std::ifstream in(mtlpath);
  if (!in) return std::string();
  std::string line;
  std::string current;
  std::string found;
  while (std::getline(in, line)) {
    if (line.size() < 2) continue;
    std::istringstream iss(line);
    std::string tag; iss >> tag;
    if (tag == "newmtl") {
      iss >> current;
    } else if (tag == "map_Kd") {
      std::string rest;
      std::getline(iss, rest);
      size_t start = rest.find_first_not_of(" \t");
      if (start == std::string::npos) continue;
      std::string tex = rest.substr(start);
      if (!tex.empty() && tex.front() == '"' && tex.back() == '"') {
        tex = tex.substr(1, tex.size()-2);
      }
      {
        std::istringstream iss2(tex);
        std::string part;
        std::string candidate;
        while (iss2 >> part) {
          candidate = part;
        }
        if (!candidate.empty()) tex = candidate;
      }
      for (char &c : tex) if (c == '\\') c = '/';

      if (current == matname) {
        found = tex;
        break;
      }
    }
  }
  return found;
}

static std::string find_first_map_in_mtl(const std::string& mtlpath)
{
  std::ifstream in(mtlpath);
  if (!in) return std::string();
  std::string line;
  std::string current;
  while (std::getline(in, line)) {
    if (line.size() < 2) continue;
    std::istringstream iss(line);
    std::string tag; iss >> tag;
    if (tag == "newmtl") {
      iss >> current;
    } else if (tag == "map_Kd") {
      std::string rest;
      std::getline(iss, rest);
      size_t start = rest.find_first_not_of(" \t");
      if (start == std::string::npos) continue;
      std::string tex = rest.substr(start);
      if (!tex.empty() && tex.front() == '"' && tex.back() == '"') {
        tex = tex.substr(1, tex.size()-2);
      }
      {
        std::istringstream iss2(tex);
        std::string part;
        std::string candidate;
        while (iss2 >> part) {
          candidate = part;
        }
        if (!candidate.empty()) tex = candidate;
      }
      for (char &c : tex) if (c == '\\') c = '/';
      return tex;
    }
  }
  return std::string();
}

static bool find_first_kd_in_mtl(const std::string& mtlpath, glm::vec3 &kd)
{
  std::ifstream in(mtlpath);
  if (!in) return false;
  std::string line;
  while (std::getline(in, line)) {
    if (line.size() < 2) continue;
    std::istringstream iss(line);
    std::string tag; iss >> tag;
    if (tag == "Kd") {
      float r,g,b; if (iss >> r >> g >> b) { kd = glm::vec3(r,g,b); return true; }
    }
  }
  return false;
}

NodePtr LoadObjNode(const std::string& filename)
{
  std::ifstream in(filename);
  if (!in) return nullptr;
  std::string line;
  std::string mtlfile;
  std::string usedmat;
  while (std::getline(in, line)) {
    if (line.size() < 2) continue;
    std::istringstream iss(line);
    std::string tag; iss >> tag;
    if (tag == "mtllib") {
      iss >> mtlfile;
    } else if (tag == "usemtl") {
      if (usedmat.empty()) iss >> usedmat;
    }
  }

  ObjPtr shp = Obj::MakeFromFile(filename);
  if (!shp) return nullptr;

  Node::Builder builder;
  builder.AddShape(shp);

  if (!mtlfile.empty() && !usedmat.empty()) {
    std::string basedir;
    auto pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) basedir = filename.substr(0,pos+1);
    std::string mtlpath = basedir + mtlfile;
    std::string tex = find_texture_in_mtl(mtlpath, usedmat);
    bool applied = false;
    if (!tex.empty()) {
      std::string texpath = basedir + tex;
      std::ifstream fin(texpath);
      if (fin) {
        fin.close();
        AppearancePtr app = Texture::Make(std::string("decal"), texpath);
        builder.AddAppearance(app);
        applied = true;
      } else {
        std::cerr << "Warning: texture not found: " << texpath << "." << std::endl;
      }
    }

    if (!applied) {
      std::string anymap = find_first_map_in_mtl(mtlpath);
      if (!anymap.empty()) {
        std::string texpath = basedir + anymap;
        std::ifstream fin(texpath);
        if (fin) {
          fin.close();
          AppearancePtr app = Texture::Make(std::string("decal"), texpath);
          builder.AddAppearance(app);
          applied = true;
        } else {
          std::cerr << "Warning: fallback texture not found: " << texpath << "." << std::endl;
        }
      }
    }

    if (!applied) {
      glm::vec3 kd;
      if (find_first_kd_in_mtl(mtlpath, kd)) {
        AppearancePtr app = Texture::Make(std::string("decal"), kd);
        builder.AddAppearance(app);
        applied = true;
      }
    }

    if (!applied) {
      std::cerr << "Warning: texture not found in MTL: " << mtlpath << ". Using white fallback." << std::endl;
      AppearancePtr app = Texture::Make(std::string("decal"), glm::vec3(1.0f,1.0f,1.0f));
      builder.AddAppearance(app);
    }
  }

  return builder.Build();
}
