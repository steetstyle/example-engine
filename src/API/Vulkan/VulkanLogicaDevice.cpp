#include <API/Vulkan/VulkanLogicalDevice.h>
#include <stdexcept>
#include <vector>
#include <algorithm>

VulkanLogicalDevice::VulkanLogicalDevice(VkPhysicalDevice physicalDevice, const QueueFamilyInformation& queueFamilyInfo) {
    float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // Collect unique valid queue families
    std::vector<uint32_t> uniqueQueueFamilies = {
        queueFamilyInfo.graphicsBitIndex,
        queueFamilyInfo.computeBitIndex,
        queueFamilyInfo.transferBitIndex,
        queueFamilyInfo.sparseBindingBitIndex,
        queueFamilyInfo.protectedBitIndex,
        queueFamilyInfo.opticalFlowBitNvIndex
    };

    // Remove invalid indices
    uniqueQueueFamilies.erase(
        std::remove(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end(), UINT32_MAX),
        uniqueQueueFamilies.end()
    );
    std::sort(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end());
    uniqueQueueFamilies.erase(std::unique(uniqueQueueFamilies.begin(), uniqueQueueFamilies.end()), uniqueQueueFamilies.end());

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &vkDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    // Retrieve queues only if indices are valid
    if (queueFamilyInfo.graphicsBitIndex != UINT32_MAX)
        vkGetDeviceQueue(vkDevice, queueFamilyInfo.graphicsBitIndex, 0, &graphicsQueue);

    if (queueFamilyInfo.computeBitIndex != UINT32_MAX)
        vkGetDeviceQueue(vkDevice, queueFamilyInfo.computeBitIndex, 0, &computeQueue);

    if (queueFamilyInfo.transferBitIndex != UINT32_MAX)
        vkGetDeviceQueue(vkDevice, queueFamilyInfo.transferBitIndex, 0, &transferQueue);

    if (queueFamilyInfo.sparseBindingBitIndex != UINT32_MAX)
        vkGetDeviceQueue(vkDevice, queueFamilyInfo.sparseBindingBitIndex, 0, &sparseBindingQueue);

    if (queueFamilyInfo.protectedBitIndex != UINT32_MAX)
        vkGetDeviceQueue(vkDevice, queueFamilyInfo.protectedBitIndex, 0, &protectedQueue);

    if (queueFamilyInfo.opticalFlowBitNvIndex != UINT32_MAX)
        vkGetDeviceQueue(vkDevice, queueFamilyInfo.opticalFlowBitNvIndex, 0, &opticalFlowQueueNV);
}


VulkanLogicalDevice::~VulkanLogicalDevice() {
    if (vkDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(vkDevice, nullptr);
    }
}

std::unique_ptr<VulkanLogicalDevice> VulkanLogicalDevice::Create(VkPhysicalDevice physicalDevice, const QueueFamilyInformation& queueFamilyInfo) {
    return std::make_unique<VulkanLogicalDevice>(physicalDevice, queueFamilyInfo);
}
