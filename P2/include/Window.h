#pragma once

#include <string>
#include "SolarSystemScene.h"
#include <memory>

class GLFWwindow;

class Window {
public:
    Window() = default;
    int Run(int width, int height, const std::string &title);
private:
    std::unique_ptr<SolarSystemScene> _solar;
    GLFWwindow *_window = nullptr;

    static void MouseButtonCallback(GLFWwindow *win, int button, int action, int mods);
    static void CursorPosInit(GLFWwindow *win, double x, double y);
    static void CursorPos(GLFWwindow *win, double x, double y);
    static void KeyCallback(GLFWwindow *win, int key, int scancode, int action, int mods);
    static void ResizeCallback(GLFWwindow *win, int width, int height);
};
