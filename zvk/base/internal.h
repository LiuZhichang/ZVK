//
// Created by 刘致畅 on 2021/10/14.
//

#ifndef VK_INTERNAL_H
#define VK_INTERNAL_H

#if     defined(ZVK_APP_USE_IMGUI)
#define ZVK_USE_IMGUI
#endif

#if     defined(ZVK_APP_USE_GLFW)
#define ZVK_USE_GLFW
#elif   defined(ZVK_APP_USE_SDL)
#define ZVK_USE_SDL
#endif

#if     defined(__APPLE__)
#define ZVK_PLATFORM_APPLE
#elif   defined(_WIN32) || (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
#define ZVK_PLATFORM_WIN32
#elif   define(__linux)
#define ZVK_PLATFORM_LINUX
#elif   defined(__ANDROID__)
#define ZVK_PLATFORM_ANDROID
#endif

#endif //VK_INTERNAL_H
