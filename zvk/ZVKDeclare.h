//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_DECLARE_H
#define VK_DECLARE_H

#include <cstdint>

namespace ZVK {

    typedef unsigned int ZVK_Bool;
    typedef std::uint8_t ZVK_QUEUE_Flags;
    typedef std::uint8_t ZVK_POOL_Flags;
    typedef std::uint8_t ZVK_Buffer_Flags;
    enum class QueueType : std::uint8_t;

    struct Context;
    struct Base;

#define ZVK_TYPEDEF(Type) \
    struct Type##_t;      \
    typedef Type##_t  *Type;

    ZVK_TYPEDEF(Window);
    ZVK_TYPEDEF(Instance)
    ZVK_TYPEDEF(Surface)
    ZVK_TYPEDEF(Queue)
    ZVK_TYPEDEF(PhysicalDevice)
    ZVK_TYPEDEF(LogicalDevice)
    ZVK_TYPEDEF(SwapChain)
    ZVK_TYPEDEF(RenderPass)
    ZVK_TYPEDEF(GraphicsPipeline)
    ZVK_TYPEDEF(Buffer)
    ZVK_TYPEDEF(Pool)
    ZVK_TYPEDEF(Allocator)

}

#undef ZVK_TYPEDEF

#endif //VK_DECLARE_H
