//
// Created by 刘致畅 on 2021/10/12.
//

#ifndef VK_SURFACE_TYPE_H
#define VK_SURFACE_TYPE_H

#include "common.h"

#include <vector>

namespace ZVK {

    char8_t PlatformSurfaceType() {
        char8_t res = 0;
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extensionCount, nullptr);
        if (!extensionCount) {
            throw std::runtime_error("failed to find extension with support current platform.");
        }

        std::vector<VkExtensionProperties> extProperties(extensionCount);
        vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extensionCount, extProperties.data());

        for (VkExtensionProperties &prop: extProperties) {
            if (strcmp(prop.extensionName, "VK_KHR_surface") == 0)
                res |= (char8_t) ZVK::SurfaceType::KHR_surface;

#if defined(__APPLE__)
            else if (strcmp(prop.extensionName, "VK_MVK_macos_surface") == 0)
                res |= (char8_t) ZVK::SurfaceType::MVK_macos_surface;
            else if (strcmp(prop.extensionName, "VK_EXT_metal_surface") == 0)
                res |= (char8_t) ZVK::SurfaceType::EXT_metal_surface;
#elif defined(__linux)
            else if (strcmp(prop.extensionName, "VK_KHR_xlib_surface") == 0)
                res |= (char8_t )VK::SurfaceType::VK_KHR_xlib_surface;
            else if (strcmp(prop.extensionName, "VK_KHR_xcb_surface") == 0)
                res |= (char8_t )VK::SurfaceType::VK_KHR_xcb_surface;
#elif defined(__WAYLAND)
            else if (strcmp(prop.extensionName, "VK_KHR_wayland_surface") == 0)
                res |= (char8_t )VK::SurfaceType::VK_KHR_wayland_surface;
//#elif defined(Andriord)
#endif
        }

        return res;
    }
}

#endif //VK_SURFACE_TYPE_H
