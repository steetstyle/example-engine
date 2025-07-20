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
    VulkanPhysicalDevice(VkInstance vulkanInstance, VkPhysicalDevice VulkanPhysicalDevice, VkSurfaceKHR vulkanSurfaceKhr);
    static std::unique_ptr<VulkanPhysicalDevice> Create(VkInstance vulkanInstance, VkPhysicalDevice vulkanPhysicalDevice, VkSurfaceKHR vulkanSurfaceKhr);
    ~VulkanPhysicalDevice();

    bool IsDeviceSuitable() const;
    VkPhysicalDevice GetPhysicalDevice() const { return vkPhysicalDevice; }
    const QueueFamilyInformation& GetQueueFamilyInformation() const { return queueFamilyInformation; }

    std::shared_ptr<VulkanLogicalDevice> VulkanPhysicalDevice::CreateLogicalDevice();
    const std::vector<std::shared_ptr<VulkanLogicalDevice>>& GetLogicalDevices() const { return logicalDevices; }

    static std::vector<VkPhysicalDevice> QueryPhysicalDevices(VkInstance vulkanInstance);
private:
    void PickQueueFamilies();
    void CheckRequiredExtensions();

private:
    VkInstance vkInstance;
    VkSurfaceKHR vkSurfaceKhr;
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    QueueFamilyInformation queueFamilyInformation;
    bool hasRequiredExtensions = false;

    std::vector<std::shared_ptr<VulkanLogicalDevice>> logicalDevices;
};

#endif // VULKANPHYSICALDEVICE_H
