//
// Created by 刘致畅 on 2021/10/14.
//

#include "ZVKContext.h"

#include "ZVKConstructor.h"
#include "ZVKDestructor.h"

VkVertexInputBindingDescription ZVK::Vertex::getBindingDescription() {
    VkVertexInputBindingDescription descript{};
    // 数组的起始地址
    descript.binding = 0;
    descript.stride = sizeof(Vertex);
    descript.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return descript;
}

std::array<VkVertexInputAttributeDescription, 4> ZVK::Vertex::getAttributeDescription() {
    std::array<VkVertexInputAttributeDescription, 4> descript{};

    // Pos
    descript[0].binding = 0;
    // rg 对应于 vec2
    descript[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    // vertex shader 中 location
    descript[0].location = 0;
    descript[0].offset = offsetof(Vertex, pos);

    descript[1].binding = 0;
    // 对应于 vec3
    descript[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    descript[1].location = 1;
    descript[1].offset = offsetof(Vertex, color);

    descript[2].binding = 0;
    // 对应于 vec3
    descript[2].format = VK_FORMAT_R32G32_SFLOAT;
    descript[2].location = 2;
    descript[2].offset = offsetof(Vertex, texCoord);

    descript[3].binding = 0;
    descript[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    descript[3].location = 3;
    descript[3].offset = offsetof(Vertex, normal);

    return descript;
}

ZVK::Context::Context() {
    ZVK::Constructor::SetupInstance(instance);
    ZVK::Constructor::SetupWindow(window);
    ZVK::Constructor::SetupSurface(surface);
    for (PhysicalDevice &_gpu: gpus) {
        ZVK::Constructor::SetupPhysicalDevice(_gpu);
    }
    ZVK::Constructor::SetupLogicalDevice(device);
    ZVK::Constructor::SetupSwapChain(swapChain);
    ZVK::Constructor::SetupRenderPass(renderPass);
    ZVK::Constructor::SetupGraphicsPipeline(pipeline);
    ZVK::Constructor::SetupPool(pool);
}

ZVK::Context::~Context() {

    /// 销毁缓冲区
//    for (Buffer &buffer: buffers) {
//        ZVK::Destructor::DestructorBuffer(device,buffer,BufferType::All,allocator);
//    }

    /// 销毁池
    ZVK::Destructor::DestructorPool(device, pool, allocator);
    /// 销毁渲染管线
    ZVK::Destructor::DestructorGraphicsPipeline(device, pipeline, allocator);
    /// 销毁渲染通道
    ZVK::Destructor::DestructorRenderPass(device, renderPass, allocator);
    /// 销毁交换链
    ZVK::Destructor::DestructorSwapChain(device, swapChain, allocator);
    /// 释放内存
    {
        delete[]deviceExtensions;
        deviceExtensions = ZVK_NULL;
    }
    /// 销毁逻辑设备
    ZVK::Destructor::DestructorLogicalDevice(device, allocator);
    /// 销毁surface
    ZVK::Destructor::DestructorSurface(instance, surface, allocator);
    /// 释放Instance资源
    ZVK::Destructor::DestructorInstance(instance, allocator);

    delete allocator;
    allocator = ZVK_NULL;
}


