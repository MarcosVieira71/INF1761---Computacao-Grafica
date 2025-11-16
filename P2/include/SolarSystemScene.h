#pragma once

#include <map>
#include <string>
#include <tuple>
#include <memory>

#include "Scene.h"
#include "Camera3D.h"



class CameraEngine;
class Orbit;
class AstralBody;
class AstralEngine;

using AstralBodyPtr = std::shared_ptr<AstralBody>;
using OrbitPtr = std::shared_ptr<Orbit>;
using CameraEnginePtr = std::shared_ptr<CameraEngine>;
using AstralEnginePtr = std::shared_ptr<AstralEngine>;


class SolarSystemScene {
public:
    SolarSystemScene();

    ScenePtr GetScene() const;
    Camera3DPtr GetCamera() const;
    Camera3DPtr GetCameraAlternative() const;
    Camera3DPtr GetActiveCamera() const;
    CameraEnginePtr GetCameraEngine() const;
    ArcballPtr GetArcball() const;
    const std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>>& GetPtrMap() const;

    void SetActiveCamera(const Camera3DPtr &c);

private:
    AstralEnginePtr CreateEngines();
    void FinalizeScene(const ShaderPtr &shader, const ShaderPtr &skyShader, OrbitPtr orbSun, AstralEnginePtr engine, const ShapePtr &sphere);

    ScenePtr _scene;
    Camera3DPtr _camera;
    Camera3DPtr _cameraAlternative;
    Camera3DPtr _activeCamera;
    CameraEnginePtr _cameraEngine;
    ArcballPtr _arcball;
    std::map<std::string, std::tuple<OrbitPtr, AstralBodyPtr>> _ptr_map;
};
