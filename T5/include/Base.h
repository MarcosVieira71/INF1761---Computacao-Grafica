#include "Node.h"
#include <memory>


class Base;
using BasePtr = std::shared_ptr<Base>;

class Orbit;
using OrbitPtr = std::shared_ptr<Orbit>;

class Appearance;
using AppearancePtr = std::shared_ptr<Appearance>;

class Base : public Node
{
public:

    struct Pivot : public Node 
    { 
        using PivotPtr = std::shared_ptr<Pivot>;
        static PivotPtr Make(float radius, float height, const glm::vec3& scale, AppearancePtr app);
        Pivot(float radius, float height, const glm::vec3& scale, AppearancePtr app);
        ~Pivot();
    };

    Pivot::PivotPtr _pivot;
    static BasePtr Make(float radius, float height, const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app);
    Base(float radius, float height, const glm::vec3& pos, const glm::vec3& scale, AppearancePtr app);
    void setup(OrbitPtr orb);

    ~Base();
};
