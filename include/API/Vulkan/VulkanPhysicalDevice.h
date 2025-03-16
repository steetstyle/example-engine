#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

struct QueueFamilyInformation {
    uint32_t graphicsBitIndex = UINT32_MAX;
    uint32_t computeBitIndex = UINT32_MAX;
    uint32_t transferBitIndex = UINT32_MAX;
    uint32_t sparseBindingBitIndex = UINT32_MAX;
    uint32_t protectedBitIndex = UINT32_MAX;
    uint32_t computeIndex = UINT32_MAX;
};

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(VkInstance vulkanInstance, VkPhysicalDevice VulkanPhysicalDevice);
    static std::unique_ptr<VulkanPhysicalDevice> Create(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice);
    ~VulkanPhysicalDevice();

    bool IsDeviceSuitable() const;
    VkPhysicalDevice GetPhysicalDevice() const { return vkPhysicalDevice; }

private:
    void PickQueueFamilies();
    void CheckRequiredExtensions();

private:
    VkInstance vkInstance;
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    QueueFamilyInformation queueFamilyInformation;
    bool hasRequiredExtensions = false;
};

#endif // VULKANPHYSICALDEVICE_H
