//
// Created by 刘致畅 on 2021/10/13.
//

#include "ext_sdl.h"

#include "../base/common.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

ZVK::ZVK_Bool ZVK::Ext_SDL::CreateWindow(ZVK::Window window) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return ZVK_FALSE;
    }

    window->whd = (void *) SDL_CreateWindow(window->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            window->width, window->height,
                                            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (window->whd == nullptr) {
        return ZVK_FALSE;
    }

    return ZVK_TRUE;
}

ZVK::ZVK_Bool
ZVK::Ext_SDL::CreateSurface(ZVK::Instance inst, ZVK::Window window, ZVK::Surface surface, ZVK::Allocator allocator) {
    if(inst == ZVK_NULL || window == ZVK_NULL || surface == ZVK_NULL) {
        return ZVK_FALSE;
    }

    if(!SDL_Vulkan_CreateSurface((SDL_Window *)window->whd, inst->handle, &surface->handle)) {
        return ZVK_FALSE;
    }

    return ZVK_TRUE;
}

void ZVK::Ext_SDL::DestroyWindow(ZVK::Window window) {
    SDL_DestroyWindow((SDL_Window *) window->whd);
}
