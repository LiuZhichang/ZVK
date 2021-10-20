//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_EXT_GLFW_H
#define VK_EXT_GLFW_H

#include "Platform.h"

namespace ZVK {

    class Ext_GLFW : public Platform_Base {

    public:
        Ext_GLFW() = default;

        Ext_GLFW(const Ext_GLFW &) = default;

        void operator=(const Ext_GLFW &) {}

        ZVK_Bool CreateWindow(ZVK::Window window) override;

        ZVK_Bool CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface,
                               ZVK::Allocator allocator = nullptr) override;

        void DestroyWindow(ZVK::Window window) override;
    };
}


#endif //VK_EXT_GLFW_H
