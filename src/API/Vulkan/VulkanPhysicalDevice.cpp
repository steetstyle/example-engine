#include <API/Vulkan/VulkanPhysicalDevice.h>
#include <stdexcept>

std::unique_ptr<VulkanPhysicalDevice> VulkanPhysicalDevice::Create(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice)
{
    std::unique_ptr<VulkanPhysicalDevice> physicalDevicePtr = std::make_unique<VulkanPhysicalDevice>(vulkanInstance, vulkanPhysicalDevice);
    if(physicalDevicePtr->IsDeviceSuitable()) return physicalDevicePtr;

    return nullptr;
}

VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice) : 
    vkInstance(vulkanInstance), vkPhysicalDevice(vulkanPhysicalDevice)
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

void VulkanPhysicalDevice::PickQueueFamilies()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);
    if (queueFamilyCount == 0) throw std::runtime_error("No queue families found.");

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
        const auto& props = queueFamilies[i];

        if ((props.queueFlags & VK_QUEUE_GRAPHICS_BIT) && queueFamilyInformation.graphicsBitIndex == UINT32_MAX) {
            queueFamilyInformation.graphicsBitIndex = i;
        }

        if ((props.queueFlags & VK_QUEUE_COMPUTE_BIT) && queueFamilyInformation.computeBitIndex == UINT32_MAX) {
            queueFamilyInformation.computeBitIndex = i;
        }

        if ((props.queueFlags & VK_QUEUE_TRANSFER_BIT) && queueFamilyInformation.transferBitIndex == UINT32_MAX) {
            queueFamilyInformation.transferBitIndex = i;
        }

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