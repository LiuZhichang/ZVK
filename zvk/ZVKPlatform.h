//
// Created by 刘致畅 on 2021/10/12.
//

#ifndef ZVK_PLATFORM_H
#define ZVK_PLATFORM_H

#if     defined(ZVK_USE_IMGUI)

#include "imgui_core/imgui.h"
#include "imgui_core/imgui_impl_vulkan.h"

#endif

#if     defined(ZVK_USE_SDL)
#include "platform/ext_sdl.h"
#include "imgui_core/imgui_impl_sdl.h"
#elif   defined(ZVK_USE_GLFW)

#include "platform/ext_glfw.h"
#include "imgui_core/imgui_impl_glfw.h"

#else
#if     defined(ZVK_PLATFORM_APPLE)
#include "platform/platform_osx.h"

#elif   defined(ZVK_PLATFORM_WIN32)
#include "platform/platform_win32.h"
#endif
#endif

#include "platform/Platform.h"

#include "ZVKNoncopyable.h"

namespace ZVK {

    class Platform : public NonCopyable {
        Platform() {
#if     defined(ZVK_USE_SDL)
            platform = new Ext_SDL;
#elif   defined(ZVK_USE_GLFW)
            platform = new Ext_GLFW;
#else
#if     defined(ZVK_PLATFORM_APPLE)
            platform = new Platform_OSX;
#elif   defined(ZVK_PLATFORM_WIN32)
            platform = new Platform_WIN32;
#endif
#endif
        }

    public :
        static Platform *Instance() {
            static Platform inst;
            return &inst;
        }

        ~Platform() {
            if (platform != nullptr) {
                delete platform;
                platform = nullptr;
            }
        }

#if defined(ZVK_USE_IMGUI)


#endif

        Platform_Base *current() {
            return platform;
        }

    private:
        Platform_Base *platform;
    };

}

#endif //ZVK_PLATFORM_H
