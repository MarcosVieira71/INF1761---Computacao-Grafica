#pragma once

#include "Node.h"

#include <glm/glm.hpp>
#include <memory>

class Appearance;
using AppearancePtr = std::shared_ptr<Appearance>;

class MoonGlobe;
using MoonGlobePtr = std::shared_ptr<MoonGlobe>;

class MoonGlobe : public Node 
{
public:
    struct Globe : public Node 
    { 
        using GlobePtr = std::shared_ptr<Globe>;
        static GlobePtr Make(AppearancePtr glassApp);
        Globe(AppearancePtr glassApp);
        ~Globe();
    };

    struct Base : public Node 
    { 
        using BasePtr = std::shared_ptr<Base>;
        static BasePtr Make(float radius, float height, const glm::vec3& scale, AppearancePtr baseApp);
        Base(float radius, float height, const glm::vec3& scale, AppearancePtr baseApp);
        ~Base();
    };


    static MoonGlobePtr Make(const glm::vec3& pos, AppearancePtr baseApp, AppearancePtr glassApp);
    MoonGlobe(const glm::vec3& pos);
    ~MoonGlobe();
};
