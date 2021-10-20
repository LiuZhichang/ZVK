//
// Created by 刘致畅 on 2021/10/13.
//

#include "ext_glfw.h"

#include "../base/common.h"

#include <GLFW/glfw3.h>

ZVK::ZVK_Bool ZVK::Ext_GLFW::CreateWindow(ZVK::Window window) {

    if (!glfwInit()) {
        return ZVK_FALSE;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window->whd = glfwCreateWindow(window->width, window->height, window->title, nullptr, nullptr);

    if (window->whd == nullptr) {
        return ZVK_FALSE;
    }

    return ZVK_TRUE;
}

ZVK::ZVK_Bool
ZVK::Ext_GLFW::CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface, ZVK::Allocator allocator) {

    if(inst == ZVK_NULL || window == ZVK_NULL || surface == ZVK_NULL) {
        return ZVK_FALSE;
    }

    VkResult result = glfwCreateWindowSurface(inst->handle, (GLFWwindow *) window->whd,
                                              (VkAllocationCallbacks *) allocator, &surface->handle);
    if (result != VK_SUCCESS) {
        return ZVK_FALSE;
    } else {
        return ZVK_TRUE;
    }
}

void ZVK::Ext_GLFW::DestroyWindow(ZVK::Window window) {
    glfwDestroyWindow((GLFWwindow *) window->whd);
    glfwTerminate();
}

