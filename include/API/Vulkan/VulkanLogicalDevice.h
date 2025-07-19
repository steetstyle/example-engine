#ifndef VULKANLOGICALDEVICE_H
#define VULKANLOGICALDEVICE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <API/Vulkan/QueueFamilyInformation.h>

class VulkanLogicalDevice {
public:
    VulkanLogicalDevice(VkPhysicalDevice physicalDevice, const QueueFamilyInformation& queueFamilyInfo);
    ~VulkanLogicalDevice();

    VkDevice GetDevice() const { return vkDevice; }
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }
    VkQueue GetPresentQueue() const { return presentQueue; }

    static std::unique_ptr<VulkanLogicalDevice> Create(VkPhysicalDevice physicalDevice, const QueueFamilyInformation& queueFamilyInfo);

private:
    VkDevice vkDevice = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkQueue computeQueue = VK_NULL_HANDLE;
    VkQueue transferQueue = VK_NULL_HANDLE;
    VkQueue sparseBindingQueue = VK_NULL_HANDLE;
    VkQueue protectedQueue = VK_NULL_HANDLE;
    VkQueue opticalFlowQueueNV = VK_NULL_HANDLE;
};

#endif // VULKANLOGICALDEVICE_H
