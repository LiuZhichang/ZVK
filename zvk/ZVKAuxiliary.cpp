//
// Created by 刘致畅 on 2021/10/13.
//

#include "ZVKAuxiliary.h"

#include "base/surfacetype.h"

#include "ZVKDestructor.h"

#include <vector>

struct PhysicalDeviceRelease {
    ZVK::PhysicalDevice gpus[ZVK::ZVK_MAX_GPU_COUNT];
    ~PhysicalDeviceRelease() {
        for(ZVK::PhysicalDevice& gpu : gpus)
            ZVK::Destructor::ReleasePhysicalDevice(gpu);
    }
};

static PhysicalDeviceRelease _phyRelease;

void ZVK::Auxiliary::RequiredInstanceExtensions(ZVK::Instance inst) {

    inst->extensionCount = 2;

    char8_t surfaceType = PlatformSurfaceType();
    if (surfaceType & (char8_t) ZVK::SurfaceType::EXT_metal_surface) {
        inst->extensions[0] = "VK_KHR_surface";
        inst->extensions[1] = "VK_EXT_metal_surface";
    } else if (surfaceType & (char8_t) ZVK::SurfaceType::MVK_macos_surface) {
        inst->extensions[0] = "VK_KHR_surface";
        inst->extensions[1] = "VK_MVK_macos_surface";
    } else if (surfaceType & (char8_t) ZVK::SurfaceType::KHR_win32_surface) {
        inst->extensions[0] = "VK_KHR_surface";
        inst->extensions[1] = "VK_KHR_win32_surface";
    }
    // TODO 添加Linux平台
}

ZVK::PhysicalDevice ZVK::Auxiliary::CheckSuitablePhysicalDevice(ZVK::PhysicalDevice devices[], uint8_t supportQueues,
                                                                const char **phyDevextensions,
                                                                uint32_t phyDevExtCount) {

    for(int i = 0; i < ZVK_MAX_GPU_COUNT; ++ i) {
        _phyRelease.gpus[i] = devices[i];
    }

    if (!phyDevextensions || !phyDevExtCount) {
        const char *extension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        phyDevextensions = &extension;
        phyDevExtCount = 1;
    }

    for (uint8_t i = 0; i < ZVK_MAX_GPU_COUNT; ++i) {
        ZVK::PhysicalDevice gpu = devices[i];

        if (!CheckPhysicalDeviceValid(gpu)) break;

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(gpu->handle, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableextensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(gpu->handle, nullptr, &extensionCount, availableextensions.data());

        uint32_t suitableCount = 0;
        for (VkExtensionProperties &prop: availableextensions) {
            for (uint32_t j = 0; j < phyDevExtCount; ++j) {
                if (strcmp(prop.extensionName, phyDevextensions[j]) == 0) {
                    ++suitableCount;
                }
            }
        }

        uint8_t flags = supportQueues;
        for (Queue &queue: gpu->queueFamilies) {
            if (queue->index != ZVK_QUEUE_INDEX_INVALID) {
                flags |= queue->type;
            }
        }

        if ((suitableCount == phyDevExtCount) && (flags & supportQueues)) {
            return gpu;
        }
    }

    return ZVK_NULL;
}

ZVK::ZVK_Bool ZVK::Auxiliary::CheckPhysicalDeviceValid(ZVK::PhysicalDevice &device) {
    return (device != ZVK_NULL) ? ZVK_TRUE : ZVK_FALSE;
}


ZVK::Queue ZVK::Auxiliary::ObtainQueue(ZVK::PhysicalDevice device, ZVK::QueueType type) {
    for (Queue &queue: device->queueFamilies) {
        if ((QueueType) queue->type == type) {
            return queue;
        }
    }

    return ZVK_NULL;
}

void ZVK::Auxiliary::CheckSuitableSurfaceAttribute(PhysicalDevice device, ZVK::Window window, ZVK::Surface surface) {
    CheckSuitableCapabilities(device, window, surface);
    CheckSuitableSurfaceFormt(device, surface);
    CheckSuitableSurfacePresent(device, surface);
}

void ZVK::Auxiliary::CheckSuitableCapabilities(ZVK::PhysicalDevice device, ZVK::Window window, ZVK::Surface surface) {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->handle, surface->handle, &surface->capabilities);

    if(surface->capabilities.currentExtent.width == UINT32_MAX) {
        VkExtent2D extent{window->width,window->height};
        extent.width = std::min(surface->capabilities.minImageExtent.width
                , std::min(surface->capabilities.maxImageExtent.width, extent.width));
        extent.height = std::min(surface->capabilities.minImageExtent.height
                , std::min(surface->capabilities.maxImageExtent.height, extent.height));

        surface->capabilities.currentExtent = extent;
    }
}

void ZVK::Auxiliary::CheckSuitableSurfaceFormt(ZVK::PhysicalDevice device, ZVK::Surface surface) {
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device->handle, surface->handle, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    if(formatCount != 0) {
        vkGetPhysicalDeviceSurfaceFormatsKHR(device->handle, surface->handle, &formatCount, formats.data());

        if( (formats.size() == 1) && (formats[0].format == VK_FORMAT_UNDEFINED) ) {
            surface->format = {VK_FORMAT_B8G8R8A8_UNORM,VK_COLORSPACE_SRGB_NONLINEAR_KHR};
            return;
        }

        for(VkSurfaceFormatKHR& surfaceFormat : formats) {
            if( (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                && (surfaceFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) ) {
                surface->format = surfaceFormat;
                return;
            }
        }

        surface->format = formats[0];
    }
}

void ZVK::Auxiliary::CheckSuitableSurfacePresent(ZVK::PhysicalDevice device, ZVK::Surface surface) {
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device->handle, surface->handle, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    if(presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device->handle, surface->handle, &presentModeCount, presentModes.data());

        VkPresentModeKHR bestPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        for(VkPresentModeKHR& presentMode : presentModes) {
            if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                surface->presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                return;
            } else if(presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                bestPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }

        surface->presentMode = bestPresentMode;
    }
}

uint32_t ZVK::Auxiliary::ObtainQueueIndex(ZVK::QueueType type) {
    uint32_t idx = -1;
    uint8_t flag = (uint8_t) type;
    while (flag) {
        flag >>= 1;
        ++idx;
    }
    return idx;
}








