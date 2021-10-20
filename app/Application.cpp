//
// Created by 刘致畅 on 2021/10/11.
//

#include "Application.h"

#define ZVK_APP_USE_GLFW
#define ZVK_APP_USE_IMGUI
#include "../zvk/ZVK.h"
#include "../zvk/log/logmanager.h"

//#include <glm/gtc/matrix_transform.hpp>


static ZVK::Logger::ptr logger = ZVK_LOG_ROOT();

static void DebugInfo(ZVK::ZVK_Bool result, const char *info) {
    ZVK_LOG_DEBUG(logger) << info;
    if(!result) {
        exit(1);
    }
}

static void check_result(ZVK::ZVK_Bool result, const char *info) {
    if (result == true) return;

    ZVK_LOG_ERROR(logger) << info;
    throw std::runtime_error(info);
}

Application::Application() {

}

Application::Application(const Application &) {

}

Application &Application::operator=(const Application &) {
    return *this;
}

void Application::run(ZVK::Context &context) {

    m_context = &context;

    initData();
    initWindow();
    initVulkan();
    mainLoop();
    cleanUp();
}

void Application::initData() {
    ZVK::Manager::RegisterDebugCallback(DebugInfo);

    m_context->pipeline->vertexShader = "/Users/liuzhichang/CLionProjects/vkExample05/shader/vert.spv";
    m_context->pipeline->fragmentShader = "/Users/liuzhichang/CLionProjects/vkExample05/shader/frag.spv";
}

void Application::initWindow() {
    ZVK_LOG_INFO(logger) << "Init GLFW window.";

    ZVK::Window window = m_context->window;

    ZVK_LOG_INFO(logger) << window->title << " " << window->width << " " << window->height;

    ZVK::ZVK_Bool result = ZVK::Platform::Instance()->current()->CreateWindow(window);
    check_result(result, "failed to create GLFW window.");
}

void Application::initVulkan() {

    ZVK::ZVK_Bool result;
//    ZVK::Manager::InitInstance(m_context);
//    ZVK::Manager::InitSurface(m_context);
//    ZVK::Manager::CheckPhysicalDevice(m_context);
//    m_context->suitableGpu = ZVK::Auxiliary::CheckSuitablePhysicalDevice(m_context->gpus);
//
//    for(ZVK::Queue& queue : m_context->suitableGpu->QueueFamilies) {
//        std::cout << "Queue Type: " << (uint)queue->Type << " Index: " << queue->Index << std::endl;
//    }
//
//    if(ZVK::Auxiliary::CheckPhysicalDeviceValid(m_context->suitableGpu)) {
//        std::cout << "find suitable GPU: " << m_context->suitableGpu->Properties.deviceName << std::endl;
//    }
//
//    ZVK::Manager::InitLogicalDevice(m_context);

//    m_context->base.gpu->QueueFamilies[0]->Priority = 1.0f;
    result = ZVK::Manager::InitContext(m_context);
}

void Application::mainLoop() {

#if defined(ZVK_APP_USE_SDL)
    SDL_Window* window = (SDL_Window*)m_context->window->whd;
    SDL_Event ev;
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&ev)) {
            if(ev.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
#elif defined(ZVK_APP_USE_GLFW)
    GLFWwindow *window = (GLFWwindow *) m_context->window->whd;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
#endif

}

void Application::cleanUp() const {

}


