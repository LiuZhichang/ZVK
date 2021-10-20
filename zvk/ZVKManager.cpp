//
// Created by 刘致畅 on 2021/10/11->
//

#include "ZVKManager.h"

#include "base/scopeguard.h"

#include "ZVKContext.h"
#include "ZVKPlatform.h"
#include "ZVKAuxiliary.h"
#include "ZVKConstructor.h"

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>

void readFile(const char *filename, std::vector<char> &code) {

    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = file.tellg();
    code.resize(fileSize);
    file.seekg(0);

    file.read(code.data(), fileSize);

}

namespace ZVK {

    Manager::DebugCallback DebugLog = ZVK_NULL;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
    {
        (void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
        std::string info;
        sprintf(info.data(),"[vulkan] ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
        ZVK::DebugLog(ZVK_TRUE, info.data());
        return VK_FALSE;
    }

    void CheckInfo(ZVK_Bool result, const std::string &info) {
        if (DebugLog) {
            DebugLog(result, info.data());
        }
    }

    template<typename T>
    ZVK_Bool TypeValid(T type) {
        return (type != ZVK_NULL) ? ZVK_TRUE : ZVK_FALSE;
    }

    VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags flags) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = image;
        createInfo.format = format;
        createInfo.subresourceRange.aspectMask = flags;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        VkImageView imageView;
        VkResult result = vkCreateImageView(device, &createInfo, nullptr, &imageView);
        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create image view.");
        }

        return imageView;
    }

    VkShaderModule createShaderModule(VkDevice device, std::vector<char>& code) {

        VkShaderModuleCreateInfo shaderModuleCreateInfo{};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.pNext = nullptr;
        shaderModuleCreateInfo.codeSize = code.size();
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);

        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create shader module.");
        }

        return shaderModule;
    }

    void Manager::RegisterDebugCallback(Manager::DebugCallback callback) {
        DebugLog = callback;
    }

    ZVK_Bool Manager::CreateInstance(ZVK::Instance inst, ZVK::Allocator allocator) {

        if (!TypeValid(inst)) {
            CheckInfo(ZVK_FALSE, "instance is uninitialized.");
            return ZVK_FALSE;
        }

        ZVK::Auxiliary::RequiredInstanceExtensions(inst);

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_0;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = inst->engine;
        appInfo.pApplicationName = inst->application;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;

        if (DebugLog != ZVK_NULL) {
            const char *layer = "VK_LAYER_KHRONOS_validation";
            createInfo.enabledLayerCount = 1;
            createInfo.ppEnabledLayerNames = &layer;

            inst->extensions[inst->extensionCount] = "VK_EXT_debug_report";
            inst->extensionCount += 1;

        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }

        createInfo.enabledExtensionCount = inst->extensionCount;
        createInfo.ppEnabledExtensionNames = (const char**)inst->extensions;

        createInfo.pApplicationInfo = &appInfo;
        VkResult result = vkCreateInstance(&createInfo, (VkAllocationCallbacks *) allocator, &inst->handle);

        if (result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create VK instance.");
            return ZVK_FALSE;
        } else {

            if(DebugLog != ZVK_NULL) {
                auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(inst->handle, "vkCreateDebugReportCallbackEXT");
                // set up the debug report callback
                VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
                debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
                debug_report_ci.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
                debug_report_ci.pfnCallback = debug_report;
                debug_report_ci.pUserData = ZVK_NULL;

                VkDebugReportCallbackEXT callback;
                result = vkCreateDebugReportCallbackEXT(inst->handle, &debug_report_ci, (VkAllocationCallbacks *) allocator, &callback);
                CheckInfo((result == VK_SUCCESS),"enable vk validation layer VK_EXT_debug_report.");
            }

            CheckInfo(ZVK_TRUE, "Create Instance successful.");
            return ZVK_TRUE;
        }
    }


    ZVK_Bool Manager::CreateInstance(Context *context) {
        return CreateInstance(context->instance, context->allocator);
    }

    ZVK_Bool Manager::CreateSurface(Instance inst, Window window, Surface surface, ZVK::Allocator allocator) {

        if (!TypeValid(inst)) {
            CheckInfo(ZVK_FALSE, "instance is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(window)) {
            CheckInfo(ZVK_FALSE, "window is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(surface)) {
            CheckInfo(ZVK_FALSE, "surface is uninitialized.");
            return ZVK_FALSE;
        }

        ZVK_Bool result = Platform::Instance()->current()->CreateSurface(inst, window, surface);

        if (result != ZVK_TRUE) {
            CheckInfo(ZVK_FALSE, "failed to create surface with support current platform.");
        } else {
            CheckInfo(ZVK_TRUE, "Create Surface successful.");
        }

        return result;
    }

    ZVK_Bool Manager::CreateSurface(Context *context) {
        return CreateSurface(context->instance, context->window, context->surface, context->allocator);
    }

    ZVK_Bool Manager::CheckPhysicalDevice(ZVK::Instance inst, Surface surface, ZVK::PhysicalDevice gpus[]) {

        if (!TypeValid(inst)) {
            CheckInfo(ZVK_FALSE, "instance is uninitialized.");
            return ZVK_FALSE;
        }
        if (!TypeValid(surface)) {
            CheckInfo(ZVK_FALSE, "surface is uninitialized.");
            return ZVK_FALSE;
        }

        ZVK_Bool result;
        uint32_t gpuCount = 0;
        result = vkEnumeratePhysicalDevices(inst->handle, &gpuCount, nullptr);
        if (!gpuCount) {
            CheckInfo(ZVK_FALSE, "failed to find GPU with support Vk.");
            return ZVK_FALSE;
        }
        std::vector<VkPhysicalDevice> _gpus(gpuCount);
        vkEnumeratePhysicalDevices(inst->handle, &gpuCount, _gpus.data());

        for (uint32_t i = 0; i < gpuCount; ++i) {

            ZVK::Constructor::SetupPhysicalDevice(gpus[i]);

            gpus[i]->handle = _gpus[i];
            PhysicalDevice &gpu = gpus[i];

            vkGetPhysicalDeviceProperties(gpu->handle, &gpu->properties);
            vkGetPhysicalDeviceMemoryProperties(gpu->handle, &gpu->memProperties);

            uint32_t propCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(gpu->handle, &propCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueProps(propCount);
            vkGetPhysicalDeviceQueueFamilyProperties(gpu->handle, &propCount, queueProps.data());

            uint32_t index = 0;
            Queue *queueFamily = gpu->queueFamilies;
            for (VkQueueFamilyProperties &queueProp: queueProps) {

                if ((queueProp.queueCount > 0)) {

                    if (queueProp.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                        uint32_t idx = ZVK::Auxiliary::ObtainQueueIndex(QueueType::Graphics);
                        queueFamily[idx]->index = i;
                    }

                    if (queueProp.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                        uint32_t idx = ZVK::Auxiliary::ObtainQueueIndex(QueueType::Compute);
                        queueFamily[idx]->index = i;
                    }

                    if (queueProp.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                        uint32_t idx = ZVK::Auxiliary::ObtainQueueIndex(QueueType::Transfer);
                        queueFamily[idx]->index = i;
                    }

                    if (queueProp.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
                        uint32_t idx = ZVK::Auxiliary::ObtainQueueIndex(QueueType::Sparse_Binding);
                        queueFamily[idx]->index = i;
                    }

                    if (queueProp.queueFlags & VK_QUEUE_PROTECTED_BIT) {
                        uint32_t idx = ZVK::Auxiliary::ObtainQueueIndex(QueueType::Protected);
                        queueFamily[idx]->index = i;
                    }

                    VkBool32 presentSupport = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(gpu->handle, i, surface->handle, &presentSupport);
                    if (presentSupport) {
                        uint32_t idx = ZVK::Auxiliary::ObtainQueueIndex(QueueType::Present);
                        queueFamily[idx]->index = i;
                    }

                    bool finish = true;
                    for (Queue &queue: gpu->queueFamilies) {
                        if (queue->index == ZVK_QUEUE_INDEX_INVALID) {
                            finish = false;
                            break;
                        }
                    }

                    if (finish) return result;
                }
            }
            ++index;
        }

        CheckInfo(ZVK_TRUE, "Check Physical Device.");

        return result;
    }

    ZVK_Bool Manager::CheckPhysicalDevice(Context *context) {
        return CheckPhysicalDevice(context->instance, context->surface, context->gpus);
    }

    ZVK_Bool Manager::CreateLogicalDevice(PhysicalDevice gpu, LogicalDevice device, ZVK_QUEUE_Flags flags,
                                          const char *const *extensions, uint32_t extCount, Allocator allocator) {

        if (!TypeValid(gpu)) {
            CheckInfo(ZVK_FALSE, "physical device is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(device)) {
            CheckInfo(ZVK_FALSE, "logical device device is uninitialized.");
            return ZVK_FALSE;
        }

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::vector<Queue> queues;
        std::map<uint32_t, float> uniqueQueueFamilies;

        int queueIdx = 0;
        while (flags) {
            if (flags & 1) {
                uint32_t index = gpu->queueFamilies[queueIdx]->index;
                float priority = gpu->queueFamilies[queueIdx]->priority;
                if(index != ZVK_QUEUE_INDEX_INVALID) {
                    queues.push_back(gpu->queueFamilies[queueIdx]);
                    std::map<uint32_t, float>::iterator it = uniqueQueueFamilies.find(index);
                    if (it != uniqueQueueFamilies.end()) {
                        if (it->second < priority) it->second = priority;
                    } else {
                        uniqueQueueFamilies.insert(std::pair<uint32_t, float>(index, priority));
                    }
                }
            }
            flags >>= 1;
            ++queueIdx;
        }

        std::map<uint32_t, float>::iterator it = uniqueQueueFamilies.begin();
        for (; it != uniqueQueueFamilies.end(); ++it) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = it->first;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &it->second;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

        gpu->features.samplerAnisotropy = VK_TRUE;
        createInfo.pEnabledFeatures = &gpu->features;

        if(DebugLog != ZVK_NULL) {
            const char* layer = "VK_LAYER_KHRONOS_validation";
            createInfo.enabledLayerCount = 1;
            createInfo.ppEnabledLayerNames = &layer;
        }

        createInfo.enabledExtensionCount = extCount;
        createInfo.ppEnabledExtensionNames = extensions;

        VkResult result = vkCreateDevice(gpu->handle, &createInfo, (VkAllocationCallbacks *) allocator,
                                         &device->handle);
        if (result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create logical device.");
            return ZVK_FALSE;
        }

        for (Queue &queue: queues) {
            vkGetDeviceQueue(device->handle, queue->index, 0, &queue->handle);
        }

        CheckInfo(ZVK_TRUE, "Create logical device successful.");

        for(int i = 0 ; i < ZVK_QUEUE_COUNT ; ++i) {
            device->queueFamilies[i] = gpu->queueFamilies[i];
        }

        return ZVK_TRUE;
    }

    ZVK_Bool Manager::CreateLogicalDevice(Context *context, ZVK_QUEUE_Flags flags) {
        return CreateLogicalDevice(context->gpu, context->device, flags,
                                   context->deviceExtensions, context->deviceExtCount,
                                   context->allocator);
    }

    ZVK_Bool
    Manager::CreateSwapChain(ZVK::Instance inst, ZVK::LogicalDevice device, ZVK::Surface surface,
                             ZVK::SwapChain swapChain, ZVK::Allocator allocator) {

        if (!TypeValid(inst)) {
            CheckInfo(ZVK_FALSE, "instance is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(device)) {
            CheckInfo(ZVK_FALSE, "logical device is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(surface)) {
            CheckInfo(ZVK_FALSE, "surface device is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(swapChain)) {
            CheckInfo(ZVK_FALSE, "swapChain device is uninitialized.");
            return ZVK_FALSE;
        }


        uint32_t imageCount = surface->capabilities.minImageCount + 1;
        if ((imageCount > 0) && (imageCount > surface->capabilities.maxImageCount)) {
            imageCount = surface->capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;

        createInfo.surface = surface->handle;

        createInfo.imageExtent = surface->capabilities.currentExtent;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surface->format.format;
        createInfo.imageColorSpace = surface->format.colorSpace;

        createInfo.imageArrayLayers = 1;

        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t graphicsQueueIdx = device->queueFamilies[ZVK::Auxiliary::ObtainQueueIndex(QueueType::Graphics)]->index;
        uint32_t presentQueueIdx = device->queueFamilies[ZVK::Auxiliary::ObtainQueueIndex(QueueType::Present)]->index;
        uint32_t queueFamilyIndices[] {
                graphicsQueueIdx,
                presentQueueIdx
        };

        if(graphicsQueueIdx != presentQueueIdx) {
            // 当图形队列和显示队列不为同一个队列时需要进行跨队列传输数据
            // ：在图形队列中绘制图像，然后传输给显示队列进行显示

            // 共享方式
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount  = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            // 互斥方式： 同一时刻图像只能被一个队列占用
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = surface->capabilities.currentTransform;

        // 是否与其他图形窗口系统进行混合操作
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = surface->presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult result = vkCreateSwapchainKHR(device->handle, &createInfo, (VkAllocationCallbacks*)allocator, &swapChain->handle);
        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create swapChain.");
            return ZVK_FALSE;
        }

        uint32_t swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(device->handle, swapChain->handle, &swapchainImageCount, nullptr);
        swapChain->imageCount = swapchainImageCount;
        swapChain->images = new VkImage [swapchainImageCount]{};
        vkGetSwapchainImagesKHR(device->handle, swapChain->handle, &swapchainImageCount, swapChain->images);

        swapChain->imageViews = new VkImageView[swapchainImageCount]{};

        for(int i = 0; i < swapChain->imageCount; ++i) {
            swapChain->imageViews[i] = createImageView(device->handle,swapChain->images[i], surface->format.format, VK_IMAGE_ASPECT_COLOR_BIT);
        }

        CheckInfo(ZVK_TRUE, "Create swapChain successful.");

        return ZVK_TRUE;
    }

    ZVK_Bool Manager::CreateSwapChain(Context *context) {
        return CreateSwapChain(context->instance,context->device,context->surface,context->swapChain,context->allocator);
    }

    ZVK_Bool Manager::CreateRenderPass(LogicalDevice device, Surface surface, RenderPass renderPass, Allocator allocator) {

        if (!TypeValid(device)) {
            CheckInfo(ZVK_FALSE, "logical device is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(surface)) {
            CheckInfo(ZVK_FALSE, "surface is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(renderPass)) {
            CheckInfo(ZVK_FALSE, "render pass is uninitialized.");
            return ZVK_FALSE;
        }

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = surface->format.format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

        // 应用在颜色和深度数据
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        // 应用在模板
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subPass{};
        subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subPass.colorAttachmentCount = 1;
        subPass.pColorAttachments = &colorAttachmentRef;
        subPass.pDepthStencilAttachment = &depthAttachmentRef;

        std::array<VkAttachmentDescription,2> attachmentDescriptions{colorAttachment,depthAttachment};

        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
        createInfo.pAttachments = attachmentDescriptions.data();
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subPass;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &dependency;

        VkResult result = vkCreateRenderPass(device->handle, &createInfo, (VkAllocationCallbacks*)allocator, &renderPass->handle);
        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create render pass.");
            return ZVK_FALSE;
        }
        CheckInfo(ZVK_TRUE, "Create render pass successful..");
        return ZVK_TRUE;
    }

    ZVK_Bool Manager::CreateRenderPass(Context *context) {
        return CreateRenderPass(context->device,context->surface,context->renderPass,context->allocator);
    }

    ZVK_Bool Manager::CreateGraphicsPipeline(LogicalDevice device, RenderPass renderPass, GraphicsPipeline pipeline, Allocator allocator) {

        if (!TypeValid(device)) {
            CheckInfo(ZVK_FALSE, "logical device is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(renderPass)) {
            CheckInfo(ZVK_FALSE, "renderPass is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(pipeline)) {
            CheckInfo(ZVK_FALSE, "render pass is uninitialized.");
            return ZVK_FALSE;
        }

        {
            std::string vertexShader = pipeline->vertexShader;
            std::string framgmentShader = pipeline->fragmentShader;
            std::string shader_info = "Shader File:\n\t" + vertexShader + "\t" + framgmentShader;
            CheckInfo(ZVK_TRUE, shader_info);
        }

        std::vector<char> vertexShaderCode;
        readFile(pipeline->vertexShader, vertexShaderCode);
        std::vector<char> fragmentShaderCode;
        readFile(pipeline->fragmentShader, fragmentShaderCode);

        if(vertexShaderCode.empty() || fragmentShaderCode.empty()) {
            CheckInfo(ZVK_FALSE, "failed to read shader file.");
            return ZVK_FALSE;
        }

        VkAllocationCallbacks* alloc = (VkAllocationCallbacks*)allocator;

        VkShaderModule vertexShaderModule = createShaderModule(device->handle,vertexShaderCode);
        VkShaderModule fragmentShaderModule = createShaderModule(device->handle, fragmentShaderCode);
        ZVK_ON_SCOPE_EXIT{
                vkDestroyShaderModule(device->handle, vertexShaderModule, alloc);
                vkDestroyShaderModule(device->handle, fragmentShaderModule, alloc);
        };

        VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo{};
        vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageCreateInfo.module = vertexShaderModule;
        vertexShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{};
        fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageCreateInfo.module = fragmentShaderModule;
        fragmentShaderStageCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStageCreateInfos[] = {
                vertexShaderStageCreateInfo,
                fragmentShaderStageCreateInfo
        };

        // step 1 顶点输入
        VkVertexInputBindingDescription vertexInputBindingDescription = Vertex::getBindingDescription();
        std::array<VkVertexInputAttributeDescription,4> vertexInputAttributeDescription = Vertex::getAttributeDescription();
        VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
        vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
        vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
        vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributeDescription.size());
        vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescription.data();

        // step 2 输入组件
        VkPipelineInputAssemblyStateCreateInfo assemblyStateCreateInfo{};
        assemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        assemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        assemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

        // step 3 视图
        VkViewport viewPort = *(VkViewport*)&pipeline->viewPort;

        // step 4 裁剪
        VkRect2D scissor = *(VkRect2D*)&pipeline->scissor;

        VkPipelineViewportStateCreateInfo viewStateCreateInfo{};
        viewStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewStateCreateInfo.scissorCount = 1;
        viewStateCreateInfo.pScissors = &scissor;
        viewStateCreateInfo.viewportCount = 1;
        viewStateCreateInfo.pViewports = &viewPort;

        // step 5 光栅化
        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
        rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationStateCreateInfo.depthClampEnable = pipeline->depthClampEnable;
        // 开启后任何图元都不会传输到光栅化阶段
        rasterizationStateCreateInfo.rasterizerDiscardEnable = pipeline->rasterizerDiscardEnable;
        // 填充模式
        rasterizationStateCreateInfo.polygonMode = static_cast<VkPolygonMode>(pipeline->polygonMode);
        rasterizationStateCreateInfo.lineWidth = pipeline->lineWidth;
        rasterizationStateCreateInfo.cullMode = pipeline->cullMode;
        rasterizationStateCreateInfo.frontFace = static_cast<VkFrontFace>(pipeline->frontFace);
        // 设置深度相关信息
        rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
        rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
        rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

        // step 6 多重采样
        VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
        multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateCreateInfo.minSampleShading = 1.0f;
        multisampleStateCreateInfo.pSampleMask = nullptr;
        multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

        // step 7 深度和模板测试
        VkPipelineDepthStencilStateCreateInfo depthStenciStateCreateInfo{};
        depthStenciStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStenciStateCreateInfo.pNext = nullptr;
        // 指定是否应该将新的深度缓冲区与深度缓冲区进行比较，以确认是否应该被丢弃
        depthStenciStateCreateInfo.depthTestEnable = VK_TRUE;
        // 指定通过深度测试的新的片段深度是否应该被实际写入深度缓冲区
        depthStenciStateCreateInfo.depthWriteEnable = VK_TRUE;
        depthStenciStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStenciStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStenciStateCreateInfo.minDepthBounds = 0.0f;
        depthStenciStateCreateInfo.maxDepthBounds = 1.0f;
        depthStenciStateCreateInfo.stencilTestEnable = VK_FALSE;
        depthStenciStateCreateInfo.front = {};
        depthStenciStateCreateInfo.back = {};

        // step 8 颜色混合
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
        colorBlendAttachmentState.blendEnable = VK_TRUE;
        colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                                   | VK_COLOR_COMPONENT_G_BIT
                                                   | VK_COLOR_COMPONENT_B_BIT
                                                   | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
        colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
        colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
        colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

        // step 9 动态修改
        // 不开启此项，会导致 规格化后坐标不正确，屏幕大小改变后其视图坐标不会变化
        VkDynamicState dynamicStates[] {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateCreateInfo.dynamicStateCount = 2;
        dynamicStateCreateInfo.pDynamicStates = dynamicStates;


        // step 10 管道布局

        VkDescriptorSetLayoutBinding uniformBinding{};
        // 对应于 vertex shader 中的 layout(binding = 0)
        uniformBinding.binding = pipeline->descriptorSetLayout->uniformBinding;
        uniformBinding.descriptorCount = 1;
        uniformBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // 表明在顶点着色器中使用
        uniformBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        // 该字段的设置与 采样有关
        uniformBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding textureBinding{};
        textureBinding.binding = pipeline->descriptorSetLayout->textureBinding;
        textureBinding.descriptorCount = 1;
        textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        textureBinding.pImmutableSamplers = nullptr;

        std::array<VkDescriptorSetLayoutBinding,2> setLayoutBindings{
                uniformBinding,
                textureBinding
        };

        VkDescriptorSetLayoutCreateInfo descriptorCreateInfo{};
        descriptorCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorCreateInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        descriptorCreateInfo.pBindings = setLayoutBindings.data();

        VkResult result = vkCreateDescriptorSetLayout(device->handle, &descriptorCreateInfo, (VkAllocationCallbacks*)allocator, &pipeline->descriptorSetLayout->handle);

        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create uniform descriptor layout.");
            ZVK_FALSE;
        }

        CheckInfo(ZVK_TRUE, "Create descriptor layout successful.");

        VkPipelineLayoutCreateInfo pipeLayoutCreateInfo{};
        pipeLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeLayoutCreateInfo.setLayoutCount = 1;
        pipeLayoutCreateInfo.pSetLayouts = &pipeline->descriptorSetLayout->handle;
        pipeLayoutCreateInfo.pushConstantRangeCount = 0;
        pipeLayoutCreateInfo.pPushConstantRanges = nullptr;

        result = vkCreatePipelineLayout(device->handle, &pipeLayoutCreateInfo, (VkAllocationCallbacks*)allocator, &pipeline->layout);
        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create pipeline layout.");
            return ZVK_FALSE;
        }

        VkGraphicsPipelineCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.stageCount = 2;
        createInfo.pStages = shaderStageCreateInfos;
        createInfo.pVertexInputState = &vertexInputStateCreateInfo;
        createInfo.pInputAssemblyState = &assemblyStateCreateInfo;
        createInfo.pViewportState = &viewStateCreateInfo;
        createInfo.pRasterizationState = &rasterizationStateCreateInfo;
        createInfo.pMultisampleState = &multisampleStateCreateInfo;
        createInfo.pDepthStencilState = &depthStenciStateCreateInfo;
        createInfo.pColorBlendState = &colorBlendStateCreateInfo;
        createInfo.pDynamicState = &dynamicStateCreateInfo;

        createInfo.renderPass = renderPass->handle;
        createInfo.subpass = 0;

        createInfo.basePipelineHandle = VK_NULL_HANDLE;
        createInfo.basePipelineIndex = 0;

        createInfo.layout = pipeline->layout;

        result = vkCreateGraphicsPipelines(device->handle, VK_NULL_HANDLE, 1, &createInfo, (VkAllocationCallbacks*)allocator, &pipeline->handle);
        if(result != VK_SUCCESS) {
            CheckInfo(ZVK_FALSE, "failed to create graphics pipeline.");
        }

        CheckInfo(ZVK_TRUE, "Create graphics pipeline successful.");
        return ZVK_TRUE;
    }

    ZVK_Bool Manager::CreateGraphicsPipeline(Context *context) {
        return CreateGraphicsPipeline(context->device, context->renderPass, context->pipeline, context->allocator);
    }

    ZVK_Bool Manager::CreatePool(LogicalDevice device, Pool pool, ZVK_POOL_Flags poolType, Allocator allocator) {

        if (!TypeValid(device)) {
            CheckInfo(ZVK_FALSE, "logical device is uninitialized.");
            return ZVK_FALSE;
        }

        if (!TypeValid(pool)) {
            CheckInfo(ZVK_FALSE, "pool is uninitialized.");
            return ZVK_FALSE;
        }

        if(poolType & (uint32_t)PoolType::Command) {
            VkCommandPoolCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            createInfo.queueFamilyIndex = device->queueFamilies[0]->index;
            createInfo.flags = 0;

            VkResult result = vkCreateCommandPool(device->handle, &createInfo, (VkAllocationCallbacks*)allocator, &pool->cmdPool);
            if(result != VK_SUCCESS) {
                CheckInfo(ZVK_FALSE, "failed to create command pool.");
                return ZVK_FALSE;
            }
            CheckInfo(ZVK_TRUE, "Create command pool successful.");
        }

        if(poolType & (uint32_t)PoolType::Descriptor) {
            std::array<VkDescriptorPoolSize, 2> poolSize{};

            poolSize[0].descriptorCount = 1;
            poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

            poolSize[1].descriptorCount = 1;
            poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

            VkDescriptorPoolCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            createInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
            createInfo.pPoolSizes = poolSize.data();
            // 最大描述符数量
            createInfo.maxSets = 1;

            VkResult result = vkCreateDescriptorPool(device->handle, &createInfo, (VkAllocationCallbacks*)allocator, &pool->descriptorPool);
            if(result != VK_SUCCESS) {
                CheckInfo(ZVK_FALSE, "failed to create descriptor pool.");
                return ZVK_FALSE;
            }
            CheckInfo(ZVK_TRUE, "Create descriptor pool successful.");
        }

        return ZVK_TRUE;
    }

    ZVK_Bool Manager::CreatePool(Context *context, ZVK_POOL_Flags poolType) {
        return CreatePool(context->device, context->pool, poolType, context->allocator);
    }

    ZVK_Bool Manager::InitContext(Context *context) {
        ZVK::ZVK_Bool result = ZVK_FALSE;
        result |= CreateInstance(context);
        result |= CreateSurface(context);
        result |= CheckPhysicalDevice(context);
        context->gpu = ZVK::Auxiliary::CheckSuitablePhysicalDevice(context->gpus);

        ZVK::Auxiliary::CheckSuitableSurfaceAttribute(context->gpu,context->window,context->surface);

        result |= ZVK::Auxiliary::CheckPhysicalDeviceValid(context->gpu);

        if (DebugLog != ZVK_NULL) {
            std::string debugInfo = "\nQueue Type-Index-valid\n\t\t";
            for (ZVK::Queue &queue: context->gpu->queueFamilies) {
                debugInfo += std::to_string(queue->type) + "\t"
                             + std::to_string(queue->index) + "\t"
                             + ((queue->index == ZVK_QUEUE_INDEX_INVALID) ? "False\n\t\t" : "True\n\t\t");
            }
            CheckInfo(ZVK_TRUE, debugInfo);

            debugInfo.clear();
            std::string GPU = context->gpu->properties.deviceName;
            debugInfo = "\nGPU Name " + GPU +
                        "\n\tvalid " + ((result == ZVK_TRUE) ? "True\n" : "False\n");
            CheckInfo(ZVK_TRUE, debugInfo);
        }

        context->gpu->queueFamilies[0]->priority = 1.0f;
        if(!context->deviceExtCount) {
            const char* extension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
            context->deviceExtensions = &extension;
            context->deviceExtCount = 1;
        }
        result |= CreateLogicalDevice(context);

        result |= CreateSwapChain(context);

        result |= CreateRenderPass(context);

        {
            context->pipeline->lineWidth = 2.0f;
            context->pipeline->viewPort = {0,0,(float)context->window->width,(float)context->window->height,0.0f,1.0f};
            context->pipeline->scissor = {0,0,context->window->width,context->window->height};
        }

        result |= CreateGraphicsPipeline(context);

        result |= CreatePool(context);

        return result;
    }



}
