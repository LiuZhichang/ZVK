//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_EXT_SDL_H
#define VK_EXT_SDL_H

#include "Platform.h"

namespace ZVK {

    class Ext_SDL : public Platform_Base {
    public:
        Ext_SDL() = default;

        Ext_SDL(const Ext_SDL &) = default;

        void operator=(const Ext_SDL &) {}

        ZVK_Bool CreateWindow(ZVK::Window window) override;

        ZVK_Bool CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface,
                               ZVK::Allocator allocator = nullptr) override;

        void DestroyWindow(ZVK::Window window) override;
    };
}


#endif //VK_EXT_SDL_H
