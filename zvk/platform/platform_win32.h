//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_PLATFORM_WIN32_H
#define VK_PLATFORM_WIN32_H

#include "Platform.h"

namespace ZVK {

    class Platform_WIN32 : public Platform_Base {

        Platform_WIN32() = default;

        Platform_WIN32(const Platform_WIN32 &) = default;

        void operator=(const Platform_WIN32 &) {}

    public:
        ZVK_Bool CreateWindow(ZVK::Window window) override;

        ZVK_Bool CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface,
                               ZVK::Allocator allocator = nullptr) override;

        void DestroyWindow(ZVK::Window window) override;
    };
}


#endif //VK_PLATFORM_WIN32_H
