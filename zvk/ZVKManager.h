//
// Created by 刘致畅 on 2021/10/11.
//

#ifndef ZVK_MANAGER_H
#define ZVK_MANAGER_H

#include "ZVKDeclare.h"
#include "ZVKInternal.h"

namespace ZVK {

    struct Base;

    class Manager {
    public:
        /*
         * @brief                回调函数
         * @param[in] result     标志
         * @param[in] info       调试信息
         * @return    void
         */
        typedef void (*DebugCallback)(ZVK_Bool result, const char *info);

        /*
         * @brief                注册调试回调函数
         * @param[in] callback   回调函数
         * @return    void
         */
        static void RegisterDebugCallback(DebugCallback callback);

        /*
         * @brief                根据实例属性，创建实例句柄
         * @param[in] inst       实例对象
         * @param[in] allocator  内存分配器
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateInstance(Instance inst, Allocator allocator = nullptr);

        /*
         * @brief                根据实例属性，创建实例句柄
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateInstance(Context *context);

        /*
         * @brief                创建表面层，依托于具体平台，以提供图形对象显示功能 [需在instance和window对象创建成功后使用]
         * @param[in] inst       实例句柄
         * @param[in] window     窗口句柄
         * @param[in] surface    表面层句柄
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateSurface(Instance inst, Window window, Surface surface,
                                      Allocator allocator = nullptr);

        /*
         * @brief                创建表面层，依托于具体平台，以提供图形对象显示功能 [需在instance和window对象创建成功后使用]
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateSurface(Context *context);

        /*
         * @brief                检测所有可用的GPU
         * @param[in] inst       实例句柄
         * @param[in] window     窗口句柄
         * @param[out]gpus       GPU对象数组
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CheckPhysicalDevice(Instance inst, Surface surface, PhysicalDevice gpus[]);

        /*
         * @brief                检测所有可用的GPU
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CheckPhysicalDevice(Context *context);

        /*
         * @brief                创建逻辑设备句柄
         * @param[in] gpu        物理设备
         * @param[in] device     逻辑设备
         * @param[in] flags      队列类型集合
         * @param[in] extensions 扩展名称
         * @param[in] extCount   扩展个数
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateLogicalDevice(PhysicalDevice gpu, LogicalDevice device, ZVK_QUEUE_Flags flags = 0x21,
                                            const char *const *extensions = nullptr, uint32_t extCount = 0,
                                            Allocator allocator = nullptr);
        /*
         * @brief                创建逻辑设备句柄
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateLogicalDevice(Context *context, ZVK_QUEUE_Flags flags = 0x21);

        /*
         * @brief                创建交换链
         * @param[in] inst       实例句柄
         * @param[in] device     逻辑设备
         * @param[in] surface    表面层
         * @param[in] swapChain  交换链
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool
        CreateSwapChain(Instance inst, LogicalDevice device, Surface surface,
                        SwapChain ,
                        Allocator allocator = nullptr);

        /*
         * @brief                创建交换链
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateSwapChain(Context* context);

        /*
         * @brief                创建渲染通道
         * @param[in] device     逻辑设备
         * @param[in] surface    表面层
         * @param[in] renderPass 渲染通道
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateRenderPass(LogicalDevice device, Surface surface, RenderPass renderPass, Allocator allocator = nullptr);

        /*
         * @brief                创建渲染通道
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateRenderPass(Context* context);

        /*
         * @brief                创建图形管线
         * @param[in] device     逻辑设备
         * @param[in] renderPass 渲染通道
         * @param[in] pipeline   图形管线
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateGraphicsPipeline(LogicalDevice device, RenderPass renderPass, GraphicsPipeline pipeline, Allocator allocator = nullptr);

        /*
         * @brief                创建图形管线
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreateGraphicsPipeline(Context* context);

        /*
         * @brief                根据指定的池类型，创建池 [可以选择多种类型]
         * @param[in] device     逻辑设备
         * @param[in] renderPass 池对象
         * @param[in] poolType   池类型
         * @param[in] allocator  内存分配器
         * @return    ZVK_Bool   创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreatePool(LogicalDevice device, Pool pool, ZVK_POOL_Flags poolType = 0x00000003, Allocator allocator = nullptr);

        /*
         * @brief                根据指定的池类型，创建池 [可以选择多种类型]
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool CreatePool(Context* context, ZVK_POOL_Flags poolType = 0x00000003);

//        static ZVK_Bool CreateBuffer(LogicalDevice device, Buffer buffer, ZVK_Buffer_Flags bufferType = 0x00000008);

        /*
         * @brief            初始化上下文，此函数只是按照默认参数初始化整个渲染步骤,建议自行更改参数，按流程创建
         * @param[in] context    上下文对象指针
         * @return ZVK_Bool      创建成功返回ZVK_TRUE，失败返回ZVK_FALSE
         */
        static ZVK_Bool InitContext(Context *context);

        Manager() = delete;

        Manager(const Manager &) = delete;

        Manager &operator=(const Manager &) = delete;
    };

}


#endif //ZVK_MANAGER_H
