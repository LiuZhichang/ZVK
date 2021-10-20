//
// Created by 刘致畅 on 2021/10/12.
//

#include "ZVKDestructor.h"

#include "ZVKContext.h"

void ZVK::Destructor::DestructorInstance(ZVK::Instance instance, ZVK::Allocator allocator) {
    vkDestroyInstance(instance->handle, (VkAllocationCallbacks *) allocator);
    delete instance;
    instance = ZVK_NULL;
}

void ZVK::Destructor::DestructorSurface(ZVK::Instance instance, ZVK::Surface surface, ZVK::Allocator allocator) {
    vkDestroySurfaceKHR(instance->handle, surface->handle, (VkAllocationCallbacks *) allocator);
    delete surface;
    surface = ZVK_NULL;
}

void
ZVK::Destructor::DestructorSwapChain(ZVK::LogicalDevice device, ZVK::SwapChain swapChain, ZVK::Allocator allocator) {
    vkDestroySwapchainKHR(device->handle, swapChain->handle, (VkAllocationCallbacks *) allocator);
    for(int i = 0 ; i < swapChain->imageCount ; ++i) {
        vkDestroyImageView(device->handle, swapChain->imageViews[i], (VkAllocationCallbacks *) allocator);
    }
    delete swapChain;
    swapChain = ZVK_NULL;
}

void ZVK::Destructor::DestructorLogicalDevice(ZVK::LogicalDevice device, ZVK::Allocator allocator) {
    vkDestroyDevice(device->handle,(VkAllocationCallbacks *) allocator);
    delete device;
    device = ZVK_NULL;
}

void ZVK::Destructor::ReleasePhysicalDevice(ZVK::PhysicalDevice gpu) {
    if(gpu != ZVK_NULL) {
        for (Queue &queue: gpu->queueFamilies) {
            delete queue;
            queue = ZVK_NULL;
        }
        delete gpu;
        gpu = ZVK_NULL;
    }
}

void ZVK::Destructor::DestructorQueue(ZVK::Queue queue) {
    delete queue;
}

void ZVK::Destructor::DestructorQueues(ZVK::Queue* queues) {
    delete[] queues;
}

void ZVK::Destructor::DestructorBuffer(ZVK::LogicalDevice device, ZVK::Buffer buffer, ZVK::BufferType type, Allocator allocator) {
    switch (type) {
        case BufferType::Normal:
            vkDestroyBuffer(device->handle, buffer->buf, (VkAllocationCallbacks*)allocator);
            break;
        case BufferType::Frame:
            vkDestroyFramebuffer(device->handle, buffer->frameBuf, (VkAllocationCallbacks*)allocator);
            break;
        case BufferType::All:
            vkDestroyBuffer(device->handle, buffer->buf, (VkAllocationCallbacks*)allocator);
            vkDestroyFramebuffer(device->handle, buffer->frameBuf, (VkAllocationCallbacks*)allocator);
            break;
    }
}

void
ZVK::Destructor::DestructorRenderPass(ZVK::LogicalDevice device, ZVK::RenderPass renderPass, ZVK::Allocator allocator) {
    vkDestroyRenderPass(device->handle,renderPass->handle,(VkAllocationCallbacks*)allocator);
    delete renderPass;
}

void ZVK::Destructor::DestructorGraphicsPipeline(ZVK::LogicalDevice device, ZVK::GraphicsPipeline pipeline, Allocator allocator) {
    vkDestroyPipeline(device->handle, pipeline->handle, (VkAllocationCallbacks*)allocator);

    vkDestroyDescriptorSetLayout(device->handle, pipeline->descriptorSetLayout->handle,(VkAllocationCallbacks*)allocator);

    delete pipeline->descriptorSetLayout;
    delete pipeline;
}

void ZVK::Destructor::DestructorPool(ZVK::LogicalDevice device, ZVK::Pool pool, ZVK::Allocator allocator) {
    vkDestroyCommandPool(device->handle, pool->cmdPool, (VkAllocationCallbacks*)allocator);
    vkDestroyDescriptorPool(device->handle, pool->descriptorPool, (VkAllocationCallbacks*)allocator);

    delete pool;
}


