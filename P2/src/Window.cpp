#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Arcball.h"
#include "CameraEngine.h"
#include "AstralBody.h"


static Window* GetWindowInstance(GLFWwindow* w) {
    return static_cast<Window*>(glfwGetWindowUserPointer(w));
}

int Window::Run(int width, int height, const std::string &title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(_window, ResizeCallback);
    glfwMakeContextCurrent(_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);

    _solar.reset(new SolarSystemScene());

    glfwSetWindowUserPointer(_window, this);
    glfwSetMouseButtonCallback(_window, MouseButtonCallback);
    glfwSetKeyCallback(_window, KeyCallback);

    float t0 = float(glfwGetTime());

    double fpsLastTime = glfwGetTime();
    int fpsFrames = 0;

    while (!glfwWindowShouldClose(_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float t = float(glfwGetTime());
        _solar->GetScene()->Update(t - t0);
        t0 = t;
        _solar->GetScene()->Render(_solar->GetActiveCamera());
        glfwSwapBuffers(_window);
        glfwPollEvents();

        fpsFrames++;
        double fpsNow = glfwGetTime();
        double elapsed = fpsNow - fpsLastTime;
        if (elapsed >= 1.0) {
            double fps = double(fpsFrames) / elapsed;
            std::string title_fps = title + " - FPS: " + std::to_string(int(fps + 0.5));
            glfwSetWindowTitle(_window, title_fps.c_str());
            fpsFrames = 0;
            fpsLastTime = fpsNow;
        }
    }

    glfwDestroyWindow(_window);
    glfwTerminate();
    return 0;
}

void Window::MouseButtonCallback(GLFWwindow *win, int button, int action, int mods)
{
    auto inst = GetWindowInstance(win);
    if (!inst) return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwSetCursorPosCallback(win, CursorPosInit);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        glfwSetCursorPosCallback(win, nullptr);
    }
}

void Window::CursorPosInit(GLFWwindow *win, double x, double y)
{
    int wn_w, wn_h, fb_w, fb_h;
    glfwGetWindowSize(win, &wn_w, &wn_h);
    glfwGetFramebufferSize(win, &fb_w, &fb_h);
    double xf = x * double(fb_w) / double(wn_w);
    double yf = (wn_h - y) * double(fb_h) / double(wn_h);
    auto inst = GetWindowInstance(win);
    if (inst && inst->_solar && inst->_solar->GetArcball())
        inst->_solar->GetArcball()->InitMouseMotion(int(xf), int(yf));
    glfwSetCursorPosCallback(win, CursorPos);
}

void Window::CursorPos(GLFWwindow *win, double x, double y)
{
    int wn_w, wn_h, fb_w, fb_h;
    glfwGetWindowSize(win, &wn_w, &wn_h);
    glfwGetFramebufferSize(win, &fb_w, &fb_h);
    double xf = x * double(fb_w) / double(wn_w);
    double yf = (wn_h - y) * double(fb_h) / double(wn_h);
    auto inst = GetWindowInstance(win);
    if (inst && inst->_solar && inst->_solar->GetArcball())
        inst->_solar->GetArcball()->AccumulateMouseMotion(int(xf), int(yf));
}

void Window::ResizeCallback(GLFWwindow *win, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::KeyCallback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) return;
    auto inst = GetWindowInstance(win);
    if (!inst || !inst->_solar) return;
    auto &sys = *inst->_solar;

    if (sys.GetCameraAlternative() && sys.GetCamera()) {
        if (sys.GetActiveCamera() == sys.GetCamera())
            sys.SetActiveCamera(sys.GetCameraAlternative());
        else
            sys.SetActiveCamera(sys.GetCamera());
    }
    if (sys.GetActiveCamera())
        sys.GetArcball();

    if (sys.GetCameraEngine()) {
        AstralBodyPtr astObservable;
        AstralBodyPtr astObserver;
        if (key == GLFW_KEY_L)
        {
            auto[orbObservable, moon] = sys.GetPtrMap().at("moon");
            auto[orbObserver, earth] = sys.GetPtrMap().at("earth");
            astObservable = moon;
            astObserver = earth;
        }
        if (key == GLFW_KEY_M)
        {
            auto[orbObservable, mars] = sys.GetPtrMap().at("mars");
            auto[orbObserver, jupiter] = sys.GetPtrMap().at("jupiter");
            astObservable = mars;
            astObserver = jupiter;
        }
        if (key == GLFW_KEY_J)
        {
            auto[orbObservable, jupiter] = sys.GetPtrMap().at("jupiter");
            auto[orbObserver, mars] = sys.GetPtrMap().at("mars");
            astObservable = jupiter;
            astObserver = mars;
        }
        if (key == GLFW_KEY_S)
        {
            auto[orbObservable, saturn] = sys.GetPtrMap().at("saturn");
            auto[orbObserver, jupiter] = sys.GetPtrMap().at("jupiter");
            astObservable = saturn;
            astObserver = jupiter;
        }
        if (key == GLFW_KEY_N)
        {
            auto[orbObservable, neptune] = sys.GetPtrMap().at("neptune");
            auto[orbObserver, jupiter] = sys.GetPtrMap().at("jupiter");
            astObservable = neptune;
            astObserver = jupiter;
        }
        if (key == GLFW_KEY_E)
        {
            auto[orbObservable, earth] = sys.GetPtrMap().at("earth");
            auto[orbObserver, venus] = sys.GetPtrMap().at("venus");
            astObservable = earth;
            astObserver = venus;
        }

        sys.GetCameraEngine()->setObservable(astObservable);
        sys.GetCameraEngine()->setObserver(astObserver);
    }
}
