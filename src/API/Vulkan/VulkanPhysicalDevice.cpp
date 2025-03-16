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
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyInformation.graphicsBitIndex = i;
            break;
        }

        if(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyInformation.computeBitIndex = i;
            break;
        }
        
        if(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyInformation.transferBitIndex = i;
            break;
        }

        if(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyInformation.sparseBindingBitIndex = i;
            break;
        }

        if(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyInformation.protectedBitIndex = i;
            break;
        }

        if(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyInformation.computeIndex = i;
            break;
        }

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
