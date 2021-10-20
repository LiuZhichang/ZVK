//
// Created by 刘致畅 on 2021/10/11.
//

#ifndef ZVKEXAMPLE05_APPLICATION_H
#define ZVKEXAMPLE05_APPLICATION_H

#include "../zvk/ZVKNoncopyable.h"
#include "../zvk/ZVKSingleton.h"

namespace ZVK {
    struct Context;
}

struct GLFWwindow;

class Application : public ZVK::NonCopyable, public ZVK::Singleton<Application> {
public:
    static Application *Instance() {
        static Application app;
        return &app;
    }

    void run(ZVK::Context &context);

private:
    void initData();

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanUp() const;

private:
    GLFWwindow *m_window;
//    ImGui_ImplVulkanH_Window    m_imguiWindow;
private:
    ZVK::Context *m_context;
private:
    Application();

    Application(const Application &);

    Application &operator=(const Application &);
};


#endif //ZVKEXAMPLE05_APPLICATION_H
