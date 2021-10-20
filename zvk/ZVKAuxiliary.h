//
// Created by 刘致畅 on 2021/10/13.
//

#ifndef VK_ZVK_AUXILIARY_H
#define VK_ZVK_AUXILIARY_H

#include "ZVKDeclare.h"

#include <vector>

namespace ZVK {
    class Auxiliary {
    public:
        /*
         * @brief           获取实例支持的扩展
         * @param   inst    实例句柄
         * @return  void
         */
        static void RequiredInstanceExtensions(ZVK::Instance inst);

        /*
         * @brief                       选择符合条件的物理设备
         * @param   devices[]           物理设备列表
         * @param   supportQueues       要支持的队列簇
         * @param   phyDevExtensions    要支持的物理设备扩展列表
         * @param   phyDevExtCount      要支持的物理设备扩展数量
         */
        static PhysicalDevice CheckSuitablePhysicalDevice(PhysicalDevice devices[],
                                                          uint8_t supportQueues = 0x21,
                                                          const char **phyDevExtensions = nullptr,
                                                          uint32_t phyDevExtCount = 0);

        static ZVK_Bool CheckPhysicalDeviceValid(PhysicalDevice &device);

        static Queue ObtainQueue(PhysicalDevice device, QueueType type);

        static void CheckSuitableCapabilities(PhysicalDevice device, Window window, Surface surface);

        static void CheckSuitableSurfaceFormt(PhysicalDevice device, Surface surface);

        static void CheckSuitableSurfacePresent(PhysicalDevice device, Surface surface);

        static void CheckSuitableSurfaceAttribute(PhysicalDevice device, Window window, Surface surface);

        static uint32_t ObtainQueueIndex(QueueType type);
    };
}


#endif //VK_ZVK_AUXILIARY_H
