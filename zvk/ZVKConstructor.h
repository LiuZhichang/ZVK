//
// Created by 刘致畅 on 2021/10/17.
//

#ifndef VK_CONSTRUCTOR_H
#define VK_CONSTRUCTOR_H

#include "base/common.h"

namespace ZVK {

    class Constructor {
    public:
#define InitType(inst, Type) \
        inst = new Type##_t;

        template<typename T>
        static void Type(T handle, HandleType type) {
            switch (type) {
#define XX(handle, type)         \
case ZVK::HandleType::type:   \
    Setup##type(handle);    \
    break;

                XX(handle, Window)
                XX(handle, Instance)
                XX(handle, Surface)
                XX(handle, PhysicalDevice)
                XX(handle, LogicalDevice)
                XX(handle, Queue)
                XX(handle, SwapChain)
                XX(handle, RenderPass)
                XX(handle, GraphicsPipeline)
                default:
                    break;
#undef XX
            }
        }

        static void SetupWindow(Window &window);

        static void SetupInstance(Instance &inst);

        static void SetupSurface(Surface &surface);

        static void SetupPhysicalDevice(PhysicalDevice &gpu);

        static void SetupLogicalDevice(LogicalDevice &device);

        static void SetupSwapChain(SwapChain &swapChain);

        static void SetupRenderPass(RenderPass &renderPass);

        static void SetupGraphicsPipeline(GraphicsPipeline &pipeline);

        static void SetupPool(Pool &pool);
#undef InitType
    };

}


#endif //VK_CONSTRUCTOR_H
