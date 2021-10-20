//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_ZVK_CONTEXT_H
#define VK_ZVK_CONTEXT_H

#include "ZVKInternal.h"
#include "base/common.h"

#include <glm/glm.hpp>
#include <array>

#if defined(ZVK_USE_GLFW)
#include "imgui_core/imgui.h"
#include "imgui_core/imgui_impl_vulkan.h"
#endif

namespace ZVK {

    struct Vertex {
        glm::vec3 pos;      // 顶点位置
        glm::vec3 color;    // 颜色
        glm::vec3 normal;   // 法线
        glm::vec2 texCoord; // 纹理坐标

        static VkVertexInputBindingDescription getBindingDescription();

        static std::array<VkVertexInputAttributeDescription,4> getAttributeDescription();

        bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
        }
    };

    struct Context {
        Instance            instance           {}        ;
        Window              window             {}        ;
        Surface             surface            {}        ;
        PhysicalDevice      gpu                {}        ;
        PhysicalDevice      gpus[ZVK_MAX_GPU_COUNT]{}    ;
        LogicalDevice       device             {}        ;
        SwapChain           swapChain          {}        ;
        RenderPass          renderPass         {}        ;
        Buffer              buffers[ZVK_MAX_BUF_COUNT]{} ;
        uint32_t            deviceExtCount     {}        ;
        const char **       deviceExtensions   {}        ;
        DescriptorSetLayout uniformBufLayout   {}        ;
        GraphicsPipeline    pipeline           {}        ;
        Pool                pool               {}        ;
        Allocator           allocator          {}        ;

        Context();
        ~Context();
    };
}

#endif //VK_ZVKCONTEXT_H
