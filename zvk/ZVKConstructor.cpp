//
// Created by 刘致畅 on 2021/10/17.
//

#include "ZVKConstructor.h"

#include <iostream>

template<typename T>
bool HandleValid(T handle) {
    return (handle != nullptr);
}

#define InitType(inst, Type) \
        inst = new Type##_t;

void ZVK::Constructor::SetupWindow(ZVK::Window &window) {
    if (HandleValid(window)) return;
    InitType(window, Window)
    {
        window->whd = ZVK_NULL;
        window->height = 800;
        window->width = 600;
        window->title = "window";
    }
}

void ZVK::Constructor::SetupInstance(ZVK::Instance &inst) {
    if (HandleValid(inst)) return;
    InitType(inst, Instance)
    {
        inst->application = "Application";
        inst->engine = "Engine";
    }
}

void ZVK::Constructor::SetupSurface(ZVK::Surface &surface) {
    if (HandleValid(surface)) return;
    InitType(surface, Surface)
    {
        surface->capabilities = VkSurfaceCapabilitiesKHR{};
        surface->format = VkSurfaceFormatKHR{};
        surface->presentMode = VkPresentModeKHR{};
    }
}

void ZVK::Constructor::SetupPhysicalDevice(ZVK::PhysicalDevice &gpu) {
    if (HandleValid(gpu)) return;
    InitType(gpu, PhysicalDevice)
    {
        gpu->features = VkPhysicalDeviceFeatures{};
        gpu->memProperties = VkPhysicalDeviceMemoryProperties{};
        gpu->properties = VkPhysicalDeviceProperties{};

        int i = 0;
        for (Queue &queue: gpu->queueFamilies) {
            InitType(queue, Queue)
            {
                queue->priority = 0.0f;
                queue->type = (1 << i);
                queue->index = ZVK_QUEUE_INDEX_INVALID;
            }
            ++i;
        }
    }
}

void ZVK::Constructor::SetupLogicalDevice(ZVK::LogicalDevice &device) {
    if (HandleValid(device)) return;
    InitType(device, LogicalDevice)
    {

    }
}

void ZVK::Constructor::SetupSwapChain(ZVK::SwapChain &swapChain) {
    if (HandleValid(swapChain)) return;
    InitType(swapChain, SwapChain)
    {
        swapChain->extent = {0, 0};
        swapChain->imageCount = 0;
        swapChain->images = ZVK_NULL;
        swapChain->imageViews = ZVK_NULL;
    }
}

void ZVK::Constructor::SetupRenderPass(ZVK::RenderPass &renderPass) {
    if (HandleValid(renderPass)) return;
    InitType(renderPass, RenderPass)
    {

    }
}

void ZVK::Constructor::SetupGraphicsPipeline(ZVK::GraphicsPipeline &pipeline) {
    if (HandleValid(pipeline)) return;
    InitType(pipeline, GraphicsPipeline)
    {
        pipeline->lineWidth = 1.0f;
        pipeline->polygonMode = ZVK::PolygonMode::Polygon_Mode_Fill;
        pipeline->rasterizerDiscardEnable = ZVK_FALSE;
        pipeline->depthClampEnable = ZVK_FALSE;
        pipeline->viewPort = {0,0,0,0,0.0f,1.0f};
        pipeline->scissor = {0,0,0,0};
        pipeline->descriptorSetLayout = new DescriptorSetLayout_t;

        pipeline->descriptorSetLayout->uniformBinding = 0;
        pipeline->descriptorSetLayout->textureBinding = 1;
    }
}

void ZVK::Constructor::SetupPool(ZVK::Pool &pool) {
    if (HandleValid(pool)) return;
    InitType(pool, Pool)
    {

    }
}


#undef InitType