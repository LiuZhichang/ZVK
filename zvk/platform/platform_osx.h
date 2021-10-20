//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_PLATFORM_OSX_H
#define VK_PLATFORM_OSX_H

#include "Platform.h"

namespace ZVK {

    class Platform_OSX : public Platform_Base {

    public:
        Platform_OSX() = default;

        Platform_OSX(const Platform_OSX &) = default;

        void operator=(const Platform_OSX &) {}

        ZVK_Bool CreateWindow(ZVK::Window window) override;

        ZVK_Bool CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface,
                               ZVK::Allocator allocator = nullptr) override;

        void DestroyWindow(ZVK::Window window) override;
    };

}

#endif //VK_PLATFORM_OSX_H
