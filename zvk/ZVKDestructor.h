//
// Created by 刘致畅 on 2021/10/12.
//

#ifndef ZVK_DESTRUCTOR_H
#define ZVK_DESTRUCTOR_H

#include "ZVKDeclare.h"
#include "base/common.h"

namespace ZVK {

    class Destructor {
    public:

        static void DestructorInstance(Instance instance, Allocator allocator = nullptr);

        static void DestructorSurface(Instance instance, Surface surface, Allocator allocator = nullptr);

        static void DestructorSwapChain(LogicalDevice device, SwapChain swapChain,
                                        Allocator allocator = nullptr);

        static void DestructorLogicalDevice(LogicalDevice logicalDevice, Allocator allocator = nullptr);

        static void ReleasePhysicalDevice(PhysicalDevice gpu);

        static void DestructorRenderPass(LogicalDevice device, RenderPass renderPass, Allocator allocator = nullptr);

        static void DestructorGraphicsPipeline(LogicalDevice device, GraphicsPipeline pipeline, Allocator allocator = nullptr);

        static void DestructorQueue(Queue queue);

        static void DestructorQueues(Queue* queues);

        static void DestructorPool(LogicalDevice device, Pool pool, Allocator allocator = nullptr);

        static void DestructorBuffer(LogicalDevice device, Buffer buffer, BufferType type = BufferType::All, Allocator allocator = nullptr);
    };
}
#endif //ZVK_DESTRUCTOR_H
