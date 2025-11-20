#ifdef _WIN32
    #include <glad/glad.h>
#elif __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
#endif

#include "Appearance.h"

#include "TexDepth.h"
#include "Shader.h"

#include <string>

class DepthAsSampler : public Appearance {
    TexDepthPtr m_smap;
    std::string m_var;
public:
    DepthAsSampler(const TexDepthPtr &smap, const std::string &varname) : m_smap(smap), m_var(varname) {}
    virtual void Load(StatePtr st) override {
        ShaderPtr shd = st->GetShader();
        shd->ActiveTexture(m_var.c_str());
        glBindTexture(GL_TEXTURE_2D, m_smap->GetTexId());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
    }
    virtual void Unload(StatePtr st) override {
        ShaderPtr shd = st->GetShader();
        shd->DeactiveTexture();
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
    }
};

