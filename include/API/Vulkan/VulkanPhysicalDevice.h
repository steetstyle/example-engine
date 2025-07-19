#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <API/Vulkan/VulkanLogicalDevice.h>
#include <API/Vulkan/QueueFamilyInformation.h>

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(VkInstance vulkanInstance, VkPhysicalDevice VulkanPhysicalDevice);
    static std::unique_ptr<VulkanPhysicalDevice> Create(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice);
    ~VulkanPhysicalDevice();

    bool IsDeviceSuitable() const;
    VkPhysicalDevice GetPhysicalDevice() const { return vkPhysicalDevice; }
    const QueueFamilyInformation& GetQueueFamilyInformation() const { return queueFamilyInformation; }

    std::shared_ptr<VulkanLogicalDevice> VulkanPhysicalDevice::CreateLogicalDevice();
    const std::vector<std::shared_ptr<VulkanLogicalDevice>>& GetLogicalDevices() const { return logicalDevices; }

private:
    void PickQueueFamilies();
    void CheckRequiredExtensions();

private:
    VkInstance vkInstance;
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    QueueFamilyInformation queueFamilyInformation;
    bool hasRequiredExtensions = false;

    std::vector<std::shared_ptr<VulkanLogicalDevice>> logicalDevices;
};

#endif // VULKANPHYSICALDEVICE_H
