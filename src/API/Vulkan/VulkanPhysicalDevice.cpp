#include <API/Vulkan/VulkanPhysicalDevice.h>
#include <stdexcept>

std::unique_ptr<VulkanPhysicalDevice> VulkanPhysicalDevice::Create(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurfaceKhr)
{
    std::unique_ptr<VulkanPhysicalDevice> physicalDevicePtr = std::make_unique<VulkanPhysicalDevice>(vulkanInstance, vulkanPhysicalDevice, vulkanSurfaceKhr);
    if(physicalDevicePtr->IsDeviceSuitable()) return physicalDevicePtr;

    return nullptr;
}

VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurfaceKhr) : 
    vkInstance(vulkanInstance), vkPhysicalDevice(vulkanPhysicalDevice), vkSurfaceKhr(vulkanSurfaceKhr)
{
    PickQueueFamilies();
    CheckRequiredExtensions();
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
    logicalDevices.clear();
}

bool VulkanPhysicalDevice::IsDeviceSuitable() const
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &deviceFeatures);

    return 
        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && 
        deviceFeatures.geometryShader &&
        queueFamilyInformation.graphicsBitIndex != UINT32_MAX && 
        hasRequiredExtensions;
}

void VulkanPhysicalDevice::PickQueueFamilies() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount == 0)
        throw std::runtime_error("No queue families found.");

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int bestGraphicsPresentIndex = -1;
    uint32_t universalGraphicsIndex = UINT32_MAX;
    uint32_t dedicatedComputeIndex = UINT32_MAX;
    uint32_t dedicatedTransferIndex = UINT32_MAX;

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        const auto& props = queueFamilies[i];

        // Check presentation support
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, vkSurfaceKhr, &presentSupport);

        // Prefer a single queue that supports both graphics and present
        if ((props.queueFlags & VK_QUEUE_GRAPHICS_BIT) && presentSupport && bestGraphicsPresentIndex == -1) {
            bestGraphicsPresentIndex = static_cast<int>(i);
        }

        // Remember the first graphics-capable queue
        if ((props.queueFlags & VK_QUEUE_GRAPHICS_BIT) && universalGraphicsIndex == UINT32_MAX) {
            universalGraphicsIndex = i;
        }

        // Find dedicated compute queue (compute but not graphics)
        if ((props.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
            !(props.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            dedicatedComputeIndex == UINT32_MAX) {
            dedicatedComputeIndex = i;
        }

        // Find dedicated transfer queue (transfer but not graphics/compute)
        if ((props.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
            !(props.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            !(props.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
            dedicatedTransferIndex == UINT32_MAX) {
            dedicatedTransferIndex = i;
        }

        // Sparse binding, protected, optical flow if available
        if ((props.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) && queueFamilyInformation.sparseBindingBitIndex == UINT32_MAX) {
            queueFamilyInformation.sparseBindingBitIndex = i;
        }
        if ((props.queueFlags & VK_QUEUE_PROTECTED_BIT) && queueFamilyInformation.protectedBitIndex == UINT32_MAX) {
            queueFamilyInformation.protectedBitIndex = i;
        }
#ifdef VK_QUEUE_OPTICAL_FLOW_BIT_NV
        if ((props.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV) && queueFamilyInformation.opticalFlowBitNvIndex == UINT32_MAX) {
            queueFamilyInformation.opticalFlowBitNvIndex = i;
        }
#endif

        // Store first presentation-only queue if not yet found
        if (presentSupport && queueFamilyInformation.presentIndex == UINT32_MAX) {
            queueFamilyInformation.presentIndex = i;
        }
    }

    // Assign graphics and present indices
    if (bestGraphicsPresentIndex != -1) {
        // Single queue supports both graphics and present
        queueFamilyInformation.graphicsBitIndex = bestGraphicsPresentIndex;
        queueFamilyInformation.presentIndex = bestGraphicsPresentIndex;
    } else {
        // Separate graphics and present queues
        if (universalGraphicsIndex == UINT32_MAX || queueFamilyInformation.presentIndex == UINT32_MAX) {
            throw std::runtime_error("Failed to find required graphics/present queue families.");
        }
        queueFamilyInformation.graphicsBitIndex = universalGraphicsIndex;
    }

    // Prefer dedicated compute queue; fallback to graphics queue
    if (dedicatedComputeIndex != UINT32_MAX) {
        queueFamilyInformation.computeBitIndex = dedicatedComputeIndex;
    } else {
        queueFamilyInformation.computeBitIndex = queueFamilyInformation.graphicsBitIndex;
    }

    // Prefer dedicated transfer queue; fallback to compute or graphics
    if (dedicatedTransferIndex != UINT32_MAX) {
        queueFamilyInformation.transferBitIndex = dedicatedTransferIndex;
    } else if (queueFamilyInformation.computeBitIndex != UINT32_MAX) {
        queueFamilyInformation.transferBitIndex = queueFamilyInformation.computeBitIndex;
    } else {
        queueFamilyInformation.transferBitIndex = queueFamilyInformation.graphicsBitIndex;
    }
}



void VulkanPhysicalDevice::CheckRequiredExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, extensions.data());

    for(const auto& extension : extensions) {
        if(strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            hasRequiredExtensions = true;
            break;
        }
    }
}

std::shared_ptr<VulkanLogicalDevice> VulkanPhysicalDevice::CreateLogicalDevice() {
    if (!IsDeviceSuitable()) {
        throw std::runtime_error("Cannot create logical device: physical device is not suitable.");
    }

    auto logicalDevice = VulkanLogicalDevice::Create(vkPhysicalDevice, queueFamilyInformation);
    logicalDevices.push_back(std::move(logicalDevice));
    return logicalDevices.back();
}

std::vector<VkPhysicalDevice> VulkanPhysicalDevice::QueryPhysicalDevices(VkInstance vulkanInstance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);

    if(deviceCount == 0) throw std::runtime_error("failed to find GPUs with Vulkan support");

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());

    return physicalDevices;
}