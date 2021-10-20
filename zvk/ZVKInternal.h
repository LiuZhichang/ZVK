//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_ZVK_INTERNAL_H
#define VK_ZVK_INTERNAL_H

#include "base/internal.h"

#if   defined(ZVK_USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#if   defined(ZVK_USE_SDL)
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#endif


/* ---------------------------------------------- *
 * |                    ImGui                   | *
 * ---------------------------------------------- */

#if   defined(ZVK_USE_IMGUI)

#include "imgui_core/imgui.h"
#include "imgui_core/imgui_impl_vulkan.h"
#if   defined(ZVK_PLATFORM_APPLE)

#endif
#if   defined(ZVK_USE_GLFW)
#include "imgui_core/imgui_impl_glfw.h"
#elif defined(ZVK_USE_SDL)
#include "imgui_core/imgui_impl_sdl.h"
#endif

#endif

#endif //VK_ZVK_INTERNAL_H
