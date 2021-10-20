//
// Created by 刘致畅 on 2021/10/14.
//

#ifndef VK_PLATFORM_BASE_H
#define VK_PLATFORM_BASE_H

#include "../ZVKDeclare.h"

namespace ZVK {

    class Platform_Base {

    public:
        Platform_Base() = default;

        Platform_Base(const Platform_Base &) = default;

        void operator=(const Platform_Base &) {}

        virtual ~Platform_Base() = default;

        virtual ZVK_Bool CreateWindow(ZVK::Window window) = 0;

        virtual ZVK_Bool CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface,
                                       ZVK::Allocator allocator = nullptr) = 0;

        virtual void DestroyWindow(ZVK::Window window) = 0;
    };

}

#endif //VK_PLATFORM_BASE_H
