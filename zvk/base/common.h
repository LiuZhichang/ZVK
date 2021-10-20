//
// Created by 刘致畅 on 2021/10/11.
//

#ifndef ZVK_COMMON_H
#define ZVK_COMMON_H

#include <vulkan/vulkan.h>
#include <cstdint>

namespace ZVK {

#define ZVK_FALSE     0U
#define ZVK_TRUE      1U
#define ZVK_NULL      nullptr

    const uint8_t ZVK_MAX_GPU_COUNT       = 8;
    const uint8_t ZVK_MAX_BUF_COUNT       = 32;
    const uint8_t ZVK_QUEUE_COUNT         = 6;
    const uint8_t ZVK_QUEUE_INDEX_INVALID = UINT8_MAX;

    typedef unsigned int ZVK_Bool;
    typedef std::uint8_t ZVK_QUEUE_Flags;

    typedef enum class PlatformType {
        GLFW    ,
        SDL     ,
        APPLE   ,
        WIN32   ,
        LINUX   ,
        ANDROID
    } PlatformType;

    typedef enum class HandleType {
        Window             ,
        Instance           ,
        Surface            ,
        PhysicalDevice     ,
        Queue              ,
        LogicalDevice      ,
        SwapChain          ,
        RenderPass         ,
        GraphicsPipeline
    } HandleType;

    typedef enum class RenderType {
        Render2D,
        Render3D
    } RenderType;

    typedef enum class BufferType {
        Normal  ,
        Frame   ,
        Command ,
        All
    } BufferType;

    typedef enum class PoolType {
        Command     = 0x00000001,
        Descriptor  = 0x00000002,
        All         = 0x00000003
    } PoolType;

    typedef enum class SurfaceType : std::uint8_t {
        KHR_surface         = 0x00000001 ,
        KHR_win32_surface   = 0x00000002 ,
        MVK_macos_surface   = 0x00000004 ,
        EXT_metal_surface   = 0x00000008 ,
        KHR_xlib_surface    = 0x00000010 ,
        KHR_xcb_surface     = 0x00000020 ,
        KHR_wayland_surface = 0x00000040
    } SurfaceType;

    typedef enum class QueueType : std::uint8_t {
        Invalid        = 0x00000000 ,
        Graphics       = 0x00000001 ,
        Compute        = 0x00000002 ,
        Transfer       = 0x00000004 ,
        Sparse_Binding = 0x00000008 ,
        Protected      = 0x00000010 ,
        Present        = 0x00000020
    } QueueType;

    typedef enum class ShaderStage {
        Stage_Vertex                    = 0x00000001,
        Stage_Tessellation_Control      = 0x00000002,
        Stage_Tessellation_Evaluation   = 0x00000004,
        Stage_Gemometry                 = 0x00000008,
        Stage_Fragment                  = 0x00000010,
        Stage_Compute                   = 0x00000020,
        Stage_All_Graphics              = 0x0000001F,
        Stage_All                       = 0x7FFFFFFF
    } ShaderStage;

    enum class PolygonMode {
        Polygon_Mode_Fill              = 0,
        Polygon_Mode_Line              = 1,
        Polygon_Mode_Point             = 2,
        Polygon_Mode_Fill_Rectangle_NV = 1000153000,
    };

    enum class FrontFace {
        Front_Face_Counter_Clockwise = 0,
        Front_Face_Clockwise         = 1,
        Front_Face_Max_Enum          = 0x7FFFFFFF
    };

    typedef struct Viewport {
        float    x;
        float    y;
        float    width;
        float    height;
        float    minDepth;
        float    maxDepth;
    } Viewport;

    typedef struct Offset2D {
        int32_t    x;
        int32_t    y;
    } Offset2D;

    typedef struct Extent2D {
        uint32_t    width {} ;
        uint32_t    height{} ;
    } Extent2D;

    typedef struct Rect2D {
        VkOffset2D    offset;
        VkExtent2D    extent;
    } Rect2D;

    typedef struct Shader {
        const char*    file;
        VkShaderModule module;
    } Shader;

    typedef struct Instance_t {
        VkInstance                           handle        {} ;
        const char *                         engine        {} ;
        const char *                         application   {} ;
        const char *                         extensions[3] {} ;
        uint32_t                             extensionCount{} ;
    } *Instance;

    typedef struct Surface_t {
        VkSurfaceKHR                         handle        {} ;
        VkSurfaceFormatKHR                   format        {} ;
        VkSurfaceCapabilitiesKHR             capabilities  {} ;
        VkPresentModeKHR                     presentMode   {} ;
    } *Surface;

    typedef struct Queue_t {
        VkQueue                              handle   {} ;
        uint8_t                              type     {} ;
        uint32_t                             index    {} ;
        float                                priority {} ;
    } *Queue;

    typedef struct PhysicalDevice_t {
        VkPhysicalDevice                     handle       {} ;
        VkPhysicalDeviceProperties           properties   {} ;
        VkPhysicalDeviceMemoryProperties     memProperties{} ;
        VkPhysicalDeviceFeatures             features     {} ;
        Queue                                queueFamilies[ZVK_QUEUE_COUNT]{} ;
    } *PhysicalDevice;

    typedef struct LogicalDevice_t {
        VkDevice                             handle       {} ;
        Queue                                queueFamilies[ZVK_QUEUE_COUNT]{} ;
    } *LogicalDevice;

    typedef struct SwapChain_t {
        VkSwapchainKHR                       handle        {} ;
        VkImage*                             images        {} ;
        VkImageView*                         imageViews    {} ;
        Extent2D                             extent        {} ;
        uint32_t                             imageCount    {} ;
    } *SwapChain;

    typedef struct RenderPass_t {
        VkRenderPass                         handle   {} ;
    } *RenderPass;

    typedef struct DescriptorSetLayout_t {
        VkDescriptorSetLayout                handle ;
        uint32_t                             uniformBinding;
        uint32_t                             textureBinding;
    } *DescriptorSetLayout;

    typedef struct GraphicsPipeline_t {
        VkPipeline                           handle                 {} ;
        VkPipelineLayout                     layout                 {} ;
        DescriptorSetLayout                  descriptorSetLayout    {} ;
        const char*                          vertexShader           {} ;
        const char*                          fragmentShader         {} ;
        Viewport                             viewPort               {} ;
        Rect2D                               scissor                {} ;
        ZVK_Bool                             depthClampEnable       {} ;
        ZVK_Bool                             rasterizerDiscardEnable{} ;
        PolygonMode                          polygonMode            {} ;
        float                                lineWidth              {} ;
        uint32_t                             cullMode               {} ;
        FrontFace                            frontFace              {} ;
    } *GraphicsPipeline;

    typedef struct Buffer_t {
        VkBuffer                             buf      {} ;
        VkFramebuffer                        frameBuf {} ;
        VkCommandBuffer                      cmdBuffer{} ;
    } *Buffer;

    typedef struct Pool_t {
        VkCommandPool                        cmdPool       {} ;
        VkDescriptorPool                     descriptorPool{} ;
    } *Pool;

    typedef struct Allocator_t {
        void*                                pUserData            {} ;
        PFN_vkAllocationFunction             pfnAllocation        {} ;
        PFN_vkReallocationFunction           pfnReallocation      {} ;
        PFN_vkFreeFunction                   pfnFree              {} ;
        PFN_vkInternalAllocationNotification pfnInternalAllocation{} ;
        PFN_vkInternalFreeNotification       pfnInternalFree      {} ;
    } *Allocator;

    typedef struct Window_t {
        void*                                whd   {};
        const char*                          title {};
        uint32_t                             width {};
        uint32_t                             height{};
    } *Window;
}

#endif //ZVK_COMMON_H
